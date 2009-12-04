
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

	for(unsigned i = 0; i < _audioSources.size(); ++i)
	{
		AudioSource* audioSource = _audioSources[i];
		std::string processingName = net.GetNetworkId(audioSource);

		const AudioSource::Ports & ports = audioSource->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			JackPort jp;
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			jp.jackPort = jack_port_register(_jackClient, portName.str().c_str(), 
											   JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0); 
			_sourceJackPorts.push_back(jp);

			audioSource->SetFrameAndHopSize(_jackBufferSize, port);
		}
	}

	for(unsigned i = 0; i < _audioSourcesBuffer.size(); ++i)
	{
		AudioSourceBuffer* audioSourceBuffer = (AudioSourceBuffer*)_audioSourcesBuffer[i];
		std::string processingName = net.GetNetworkId(audioSourceBuffer);

		const AudioSourceBuffer::Ports & ports = audioSourceBuffer->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			JackPort jp;
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			jp.jackPort = jack_port_register(_jackClient, portName.str().c_str(), 
											   JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0); 
			_sourceJackPorts.push_back(jp);

			audioSourceBuffer->SetFrameAndHopSize(_jackBufferSize, port);
		}
	}
}

void JACKNetworkPlayer::RegisterOutputPorts(const Network& net)
{
	CLAM_ASSERT( _sinkJackPorts.empty(),
		"JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");	

	for(unsigned i = 0; i < _audioSinks.size(); ++i)
	{
		AudioSink* audioSink = _audioSinks[i];
		std::string processingName = net.GetNetworkId(audioSink);

		const AudioSink::Ports & ports = audioSink->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			JackPort jp;
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			jp.jackPort=jack_port_register(_jackClient, portName.str().c_str(),
											  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
			_sinkJackPorts.push_back(jp);

			audioSink->SetFrameAndHopSize(_jackBufferSize, port);
		}
	}

	for(unsigned i = 0; i < _audioSinksBuffer.size(); ++i)
	{
		AudioSinkBuffer* audioSinkBuffer = (AudioSinkBuffer*)_audioSinksBuffer[i];
		std::string processingName = net.GetNetworkId(audioSinkBuffer);

		const AudioSinkBuffer::Ports & ports = audioSinkBuffer->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			JackPort jp;
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			jp.jackPort=jack_port_register(_jackClient, portName.str().c_str(),
											  JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
			_sinkJackPorts.push_back(jp);

			audioSinkBuffer->SetFrameAndHopSize(_jackBufferSize, port);
		}
	}
}

void JACKNetworkPlayer::UnRegisterPorts()
{
	for (JackPorts::iterator it=_sinkJackPorts.begin(); it!=_sinkJackPorts.end(); it++)
	{
		if ( jack_port_unregister ( _jackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	_sinkJackPorts.clear();
	
	for (JackPorts::iterator it=_sourceJackPorts.begin(); it!=_sourceJackPorts.end(); it++)
	{
		if ( jack_port_unregister ( _jackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	_sourceJackPorts.clear();
}
    
void JACKNetworkPlayer::CopyJackBuffersToGenerators(const jack_nframes_t nframes)
{
	unsigned buffer = 0;
	for (unsigned i = 0; i < _audioSources.size(); ++i)
	{
		unsigned port_size = _audioSources[i]->GetPorts().size();
		for(unsigned port = 0; port < port_size; ++port)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackInBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer(_sourceJackPorts[buffer++].jackPort, nframes);

			//Tell the AudioSource where to look for data in its Do()
			_audioSources[i]->SetExternalBuffer(jackInBuffer, nframes, port);
		}	
	}
	
	for (unsigned i = 0; i < _audioSourcesBuffer.size(); ++i)
	{
		AudioSourceBuffer* audioSourcesBuffer = (AudioSourceBuffer*)_audioSourcesBuffer[i];

		unsigned port_size = audioSourcesBuffer->GetPorts().size();
		for(unsigned port = 0; port < port_size; ++port)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackInBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer(_sourceJackPorts[buffer++].jackPort, nframes);

			//Tell the AudioSource where to look for data in its Do()
			audioSourcesBuffer->SetExternalBuffer(jackInBuffer, nframes, port);
		}	
	}
}

void JACKNetworkPlayer::CopySinksToJackBuffers(const jack_nframes_t nframes)
{
	unsigned buffer = 0;
	for (unsigned i = 0; i < _audioSinks.size(); ++i)
	{
		unsigned port_size = _audioSinks[i]->GetPorts().size();
		for(unsigned port = 0; port < port_size; ++port)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t* jackOutBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer(_sinkJackPorts[buffer++].jackPort, nframes);

			//Tell the AudioSource where to copy data consumed in its Do()
			_audioSinks[i]->SetExternalBuffer(jackOutBuffer, nframes, port);	
		}
	}

	for (unsigned i = 0; i < _audioSinksBuffer.size(); ++i)
	{
		AudioSinkBuffer* audioSinkBuffer = (AudioSinkBuffer*)_audioSinksBuffer[i];

		unsigned port_size = audioSinkBuffer->GetPorts().size();
		for(unsigned port = 0; port < port_size; ++port)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t* jackOutBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer(_sinkJackPorts[buffer++].jackPort, nframes);

			//Tell the AudioSource where to copy data consumed in its Do()
			audioSinkBuffer->SetExternalBuffer(jackOutBuffer, nframes, port);	
		}
	}
}

void JACKNetworkPlayer::BlankJackBuffers(const jack_nframes_t nframes)
{
	for (JackPorts::iterator it=_sinkJackPorts.begin(); it!=_sinkJackPorts.end(); it++)
	{
		jack_default_audio_sample_t *jackOutBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
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

	CopyJackBuffersToGenerators(nframes);
	CopySinksToJackBuffers(nframes);
	GetNetwork().Do();
}

//Saves the connections made to our local in/out jack ports
void JACKNetworkPlayer::StoreConnections()
{
	for (JackPorts::iterator it=_sourceJackPorts.begin(); it!=_sourceJackPorts.end(); it++)
	{
		JackConnection connection;
		connection.processingName = it->PortName();
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
		_incomingJackConnections.push_back(connection);
	}
	
	for (JackPorts::iterator it=_sinkJackPorts.begin(); it!=_sinkJackPorts.end(); it++)
	{
		JackConnection connection;
		connection.processingName = it->PortName();
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
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
	//std::cout << "Automatically connecting to JACK input and output ports" << std::endl;
	
	//CONNECT JACK OUTPUT PORTS TO CLAM EXTERNGENERATORS
	const char ** portnames= jack_get_ports ( _jackClient , _jackOutPortAutoConnectList.c_str(), NULL, JackPortIsOutput);

	if (not portnames)
	{
		std::cout << " -WARNING: couldn't locate any JACK output port <"
			<< _jackOutPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		int i=0;

		//Double iterate AudioSources & found JACK out ports
		for ( JackPorts::iterator it= _sourceJackPorts.begin(); it!=_sourceJackPorts.end(); it++)
		{
			std::cout << "- Connecting " << portnames[i] << " -> " 
				<< it->PortName() << std::endl;

			if ( jack_connect( _jackClient, portnames[i], it->PortName() ) !=0 )
				std::cerr << " -WARNING: couldn't connect" << std::endl;
		
			i++;
			if (portnames[i]==NULL) break;
		}	
	}		
	free(portnames);

	//CONNECT CLAM EXTERNSINKS TO JACK INPUT PORTS
	portnames= jack_get_ports ( _jackClient , _jackInPortAutoConnectList.c_str(), NULL, JackPortIsInput);

	if ( not portnames)
	{
		std::cout << " -WARNING: couldn't locate any JACK input port <"
			<< _jackInPortAutoConnectList << ">"<<std::endl;
		return;
	}
	int i=0;

	//Double iterate found JACK in ports & ExterSinks
	for (JackPorts::iterator it= _sinkJackPorts.begin(); it!=_sinkJackPorts.end(); it++)
	{
		std::cout << "- Connecting "<< it->PortName()
			<< " -> " << portnames[i] << std::endl;

		if ( jack_connect( _jackClient, it->PortName(), portnames[i]) != 0)
			std::cerr << " -WARNING: couldn't connect" << std::endl;
		i++;
		if (portnames[i]==NULL) break;
	}
	free(portnames);
}

} //namespace CLAM


