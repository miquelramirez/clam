
#include "JACKNetworkPlayer.hxx"
#include "PushFlowControl.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

#include <iostream>

namespace CLAM
{

//JACK CODE
inline int JackProcessingCallback (jack_nframes_t nframes, void *arg)
{
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	player->Do(nframes);
	return 0;
}

inline void JackShutdownCallback (void *arg)
{
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	player->OnShutdown();
}

JACKNetworkPlayer::JACKNetworkPlayer(const std::string & name)
	: _jackClient(0)
	, _jackClientName(name)
{
	_autoConnect=false;
	_jackClient=0;
	InitClient();
}

JACKNetworkPlayer::~JACKNetworkPlayer()
{
	Stop();

	if (not _jackClient) return;
	bool error = jack_client_close (_jackClient);
	if (error)
	{
		std::cerr << "JACK ERROR: cannot close client" << std::endl;
		exit(1);
	}
}

bool JACKNetworkPlayer::IsWorking()
{
	return _jackClient != 0;
}

std::string JACKNetworkPlayer::NonWorkingReason()
{
	if (_jackClient) return "";
	return "No connection to JACK server available";
}

void JACKNetworkPlayer::InitClient()
{
	unsigned jackClientNameMaxSize=jack_client_name_size();
	if (jackClientNameMaxSize<=_jackClientName.size()) // the = is because the 0 of the c string...
	{
		std::cerr << "JACK WARNING: jack client name \"" << _jackClientName
			<<"\" truncated to " << jackClientNameMaxSize << " characters"<<std::endl;
		_jackClientName.resize(jackClientNameMaxSize-1);
	}

	CLAM_ASSERT(not _jackClient, "JACKNetworkPlayer: Initializing a client without closing the previous one");
	jack_status_t jackStatus;
	_jackClient = jack_client_open ( _jackClientName.c_str(), JackNullOption, &jackStatus );
	if (not _jackClient)
	{
		// TODO: Check jackStatus to be more informative
		std::cerr << "JACK ERROR: server not running?"<< std::endl;
		return;
	}

	//Register callback method for processing
	bool err = jack_set_process_callback (_jackClient, JackProcessingCallback, this);
	CLAM_ASSERT(not err, "JACK ERROR: registering process callbacks");

	//Register shutdown callback
	jack_on_shutdown (_jackClient, JackShutdownCallback, this);

	//Get JACK information
	_jackSampleRate=(int)jack_get_sample_rate (_jackClient);
	_jackBufferSize=(int)jack_get_buffer_size (_jackClient);
}


void JACKNetworkPlayer::RegisterPorts()
{
	RegisterInputPorts( GetNetwork() );
	RegisterOutputPorts( GetNetwork() );
}

void JACKNetworkPlayer::RegisterInputPorts(const Network& net)
{
	CLAM_ASSERT( _sourceJackPorts.empty(),
		"JACKNetworkPlayer::RegisterInputPorts() : there are already registered input ports");
	unsigned nPorts = GetNSources();
	_sourceJackPorts.resize(nPorts);
	for (unsigned i=0; i<nPorts; i++)
	{
		_sourceJackPorts[i] = jack_port_register(
			_jackClient, SourceName(i).c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
		SetSourceFrameSize(i,_jackBufferSize);
	}
}

void JACKNetworkPlayer::RegisterOutputPorts(const Network& net)
{
	CLAM_ASSERT( _sinkJackPorts.empty(),
		"JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");
	unsigned nPorts = GetNSinks();
	_sinkJackPorts.resize(nPorts);
	for(unsigned i = 0; i < nPorts; ++i)
	{
		_sinkJackPorts[i] = jack_port_register(
			_jackClient, SinkName(i).c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
		SetSinkFrameSize(i, _jackBufferSize);
	}
}

void JACKNetworkPlayer::UnRegisterPorts()
{
	for (unsigned i=0; i<_sinkJackPorts.size(); i++)
	{
		if ( jack_port_unregister ( _jackClient, _sinkJackPorts[i]) )
		{
			std::cerr << "JACK ERROR: unregistering port " << PortName(_sinkJackPorts[i]) << std::endl;
			exit(1);
		}
	}
	_sinkJackPorts.clear();

	for (unsigned i=0; i<_sourceJackPorts.size(); i++)
	{
		if ( jack_port_unregister ( _jackClient, _sourceJackPorts[i]) )
		{
			std::cerr << "JACK ERROR: unregistering port " << PortName(_sourceJackPorts[i]) << std::endl;
			exit(1);
		}
	}
	_sourceJackPorts.clear();
}

void JACKNetworkPlayer::CopyJackBuffersToSources(const jack_nframes_t nframes)
{
	for (unsigned i=0; i<_sourceJackPorts.size(); i++)
	{
		jack_default_audio_sample_t *jackInBuffer =
			(jack_default_audio_sample_t*) jack_port_get_buffer(_sourceJackPorts[i], nframes);
		SetSourceBuffer(i, jackInBuffer, nframes);
	}
}

void JACKNetworkPlayer::CopySinksToJackBuffers(const jack_nframes_t nframes)
{
	for (unsigned i=0; i<_sinkJackPorts.size(); i++)
	{
		jack_default_audio_sample_t* jackOutBuffer =
			(jack_default_audio_sample_t*) jack_port_get_buffer(_sinkJackPorts[i], nframes);
		SetSinkBuffer(i, jackOutBuffer, nframes);
	}
}

void JACKNetworkPlayer::BlankJackBuffers(const jack_nframes_t nframes)
{
	for (unsigned i=0; i<_sinkJackPorts.size(); i++)
	{
		jack_default_audio_sample_t *jackOutBuffer =
			(jack_default_audio_sample_t*) jack_port_get_buffer ( _sinkJackPorts[i], nframes);
		std::memset(jackOutBuffer, 0, nframes*sizeof(jack_default_audio_sample_t));
	}
}

void JACKNetworkPlayer::Start()
{
	if (IsPlaying()) return;
	if (IsPaused())
	{
		BePlaying();
		return;
	}
	if (!_jackClient) InitClient();
	if (!_jackClient) return;

	CacheSourcesAndSinks();

	BePlaying();
	UnRegisterPorts();
	RegisterPorts();
	GetNetwork().Start();

	//JACK CODE (the init order of network, ... should be decided)
	if (jack_activate (_jackClient)) {
		std::cerr << "JACK ERROR: cannot activate client" << std::endl;
		exit(1);
	}

	if (_autoConnect)
		AutoConnectPorts();
	else
		RestoreConnections();
}
void JACKNetworkPlayer::Init()
{
	if (not _jackClient) InitClient();
}
void JACKNetworkPlayer::OnShutdown()
{
	if (not _jackClient) return;
	BeStopped();
	GetNetwork().Stop();
	_sinkJackPorts.clear(); // TODO: May we save them?
	_sourceJackPorts.clear(); // TODO: May we save them?;
	_jackClient=0;
}

void JACKNetworkPlayer::Stop()
{
	if (IsStopped()) return;
	StoreConnections();
	if ( jack_deactivate (_jackClient) )
	{
		std::cerr << "JACK ERROR: cannot deactivate client" << std::endl;
		exit(1);
	}
	BeStopped();
	GetNetwork().Stop();
}

void JACKNetworkPlayer::Do(const jack_nframes_t nframes)
{
	if (IsStopped()) return;
	if (IsPaused())
	{
		BlankJackBuffers(nframes);
		return;
	}

	CopyJackBuffersToSources(nframes);
	CopySinksToJackBuffers(nframes);
	GetNetwork().Do();
}

//Saves the connections made to our local in/out jack ports
void JACKNetworkPlayer::StoreConnections()
{
	for (unsigned i=0; i<_sourceJackPorts.size(); i++)
	{
		JackConnection connection;
		connection.processingName = PortName(_sourceJackPorts[i]);
		connection.outsideConnections = jack_port_get_connections ( _sourceJackPorts[i] );
		_incomingJackConnections.push_back(connection);
	}

	for (unsigned i=0; i<_sinkJackPorts.size(); i++)
	{
		JackConnection connection;
		connection.processingName = PortName(_sinkJackPorts[i]);
		connection.outsideConnections = jack_port_get_connections ( _sinkJackPorts[i] );
		_outgoingJackConnections.push_back(connection);
	}
}

//Loads the connections made to our local in/out jack ports
void JACKNetworkPlayer::RestoreConnections()
{
	for (JackConnections::iterator it=_incomingJackConnections.begin(); it!=_incomingJackConnections.end(); it++)
	{
		if (not it->outsideConnections) continue;
		for (unsigned i=0; it->outsideConnections[i]; i++)
		{
			bool error = jack_connect ( _jackClient, it->outsideConnections[i], it->processingName.c_str() );
			if (error)
				std::cerr << "JACK WARNING: could not reconnect ports ( " <<
					it->processingName << " , " << it->outsideConnections[i] << " )" <<std::endl;
		}
		free(it->outsideConnections);
	}
	_incomingJackConnections.clear();

	for (JackConnections::iterator it=_outgoingJackConnections.begin(); it!=_outgoingJackConnections.end(); it++)
	{
		if (not it->outsideConnections) continue;
		for (unsigned i=0; it->outsideConnections[i]; i++)
		{
			bool error = jack_connect ( _jackClient, it->processingName.c_str(), it->outsideConnections[i] );
			if (error)
				std::cerr << "JACK WARNING: could not reconnect ports ( " <<
					it->outsideConnections[i] << " , " << it->processingName << " )" <<std::endl;
		}
		free(it->outsideConnections);
	}
	_outgoingJackConnections.clear();
}

void JACKNetworkPlayer::AutoConnectPorts()
{
	//Automatically connect the ports to external jack ports

	//CONNECT JACK OUTPUT PORTS TO CLAM EXTERNGENERATORS
	const char ** portnames= jack_get_ports ( _jackClient , _jackOutPortAutoConnectList.c_str(), NULL, JackPortIsOutput);
	if (not portnames)
	{
		std::cout << " -WARNING: couldn't locate any JACK output port <"
			<< _jackOutPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		for (unsigned i=0; i<_sourceJackPorts.size() && portnames[i]; i++)
		{
			jack_port_t * port = _sourceJackPorts[i];
			std::cout << "- Connecting " << portnames[i] << " -> "
				<< PortName(port) << std::endl;

			if ( jack_connect( _jackClient, portnames[i], PortName(port) ) !=0 )
				std::cerr << " -WARNING: couldn't connect" << std::endl;
		}
		free(portnames);
	}

	//CONNECT CLAM EXTERNSINKS TO JACK INPUT PORTS
	portnames= jack_get_ports ( _jackClient , _jackInPortAutoConnectList.c_str(), NULL, JackPortIsInput);
	if ( not portnames)
	{
		std::cout << " -WARNING: couldn't locate any JACK input port <"
			<< _jackInPortAutoConnectList << ">"<<std::endl;
	}
	else 
	{
		for (unsigned i=0; i<_sinkJackPorts.size() && portnames[i]; i++)
		{
			const char * localPortName = PortName(_sinkJackPorts[i]);
			std::cout << "- Connecting "<< localPortName
				<< " -> " << portnames[i] << std::endl;

			if ( jack_connect( _jackClient, localPortName, portnames[i]) != 0)
				std::cerr << " -WARNING: couldn't connect" << std::endl;
		}
		free(portnames);
	}
}

} //namespace CLAM


