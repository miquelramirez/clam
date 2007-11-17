
#include "JACKNetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{

//JACK CODE
inline int JackProcessingCallback (jack_nframes_t nframes, void *arg)
{
std::cout << "\n++++ begin callback"<<std::endl;
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	player->Do(nframes);
std::cout << "++++ end callback\n"<<std::endl;

	return 0;
}

inline void JackShutdownCallback (void *arg)
{
	JACKNetworkPlayer* player=(JACKNetworkPlayer*)arg;
	player->OnShutdown();
}

JACKNetworkPlayer::JACKNetworkPlayer(const std::string & name)
	: _jackClientName(name)
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

bool JACKNetworkPlayer::IsWorking() const
{
	return _jackClient != 0;
}

std::string JACKNetworkPlayer::NonWorkingReason() const
{
	if (_jackClient) return "";
	return "No connection to JACK server available";
}

void JACKNetworkPlayer::InitClient()
{
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
	CLAM_ASSERT( _sourceJackBindings.empty(), "JACKNetworkPlayer::RegisterInputPorts() : there are already registered input ports");
	
	SourceJackBinding pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "AudioSource") continue;

		//Get Processing address
		pair.source=(AudioSource*)it->second;
		pair.source->SetFrameAndHopSize(_jackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (_jackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		//Add the pair (jack port, clam jack receiver) to the list
		_sourceJackBindings.push_back(pair);
	}
}

void JACKNetworkPlayer::RegisterOutputPorts(const Network& net)
{
	CLAM_ASSERT( _sinkJackBindings.empty(), "JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");
	
	SinkJackBinding pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "AudioSink") continue;

		//Get Processing address
		pair.sink=(AudioSink*)it->second;
		pair.sink->SetFrameAndHopSize(_jackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (_jackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

		//Add the pair (jack port, clam jack receiver) to the list
		_sinkJackBindings.push_back(pair);
	}
}

void JACKNetworkPlayer::UnRegisterPorts()
{
	for (SinkJackBindings::iterator it=_sinkJackBindings.begin(); it!=_sinkJackBindings.end(); it++)
	{
		if ( jack_port_unregister ( _jackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	_sinkJackBindings.clear();
	
	for (SourceJackBindings::iterator it=_sourceJackBindings.begin(); it!=_sourceJackBindings.end(); it++)
	{
		if ( jack_port_unregister ( _jackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	_sourceJackBindings.clear();
}

void JACKNetworkPlayer::CopyJackBuffersToGenerators(const jack_nframes_t nframes)
{
	for (SourceJackBindings::iterator it=_sourceJackBindings.begin(); it!=_sourceJackBindings.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackInBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the AudioSource where to look for data in its Do()
		it->source->SetExternalBuffer( jackInBuffer, nframes );

	}

}

void JACKNetworkPlayer::CopySinksToJackBuffers(const jack_nframes_t nframes)
{
	for (SinkJackBindings::iterator it=_sinkJackBindings.begin(); it!=_sinkJackBindings.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackOutBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the AudioSource where to copy data consumed in its Do()
		it->sink->SetExternalBuffer( jackOutBuffer, nframes);	
	}
}

void JACKNetworkPlayer::Start()
{
	if (!IsStopped()) return;

	if (!_jackClient) InitClient();
	if (!_jackClient) return;
	
	SetStopped(false);
	
	UnRegisterPorts();
	RegisterPorts();
		
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
	InitClient();
}
void JACKNetworkPlayer::OnShutdown()
{
	if (not _jackClient) return;
	SetStopped(true);
	GetNetwork().Stop();
	_sinkJackBindings.clear(); // TODO: May we save them?
	_sourceJackBindings.clear(); // TODO: May we save them?;
	_jackClient=0;
}

void JACKNetworkPlayer::Stop()
{
	if (IsStopped()) return;
	SetStopped(true);

	StoreConnections();
	
	if ( jack_deactivate (_jackClient) )
	{
		std::cerr << "JACK ERROR: cannot deactivate client" << std::endl;
		exit(1);
	}
}

void JACKNetworkPlayer::Do(const jack_nframes_t nframes)
{
	if (IsStopped()) return;

	CopyJackBuffersToGenerators(nframes);
	CopySinksToJackBuffers(nframes);
	GetNetwork().Do();
}

//Saves the connections made to our local in/out jack ports
void JACKNetworkPlayer::StoreConnections()
{
	for (SourceJackBindings::iterator it=_sourceJackBindings.begin(); it!=_sourceJackBindings.end(); it++)
	{
		JackConnection connection;
		connection.processingName = it->PortName();
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
		_incomingJackConnections.push_back(connection);
	}
	
	for (SinkJackBindings::iterator it=_sinkJackBindings.begin(); it!=_sinkJackBindings.end(); it++)
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
	std::cout << "Automatically connecting to JACK input and output ports" << std::endl;
	
	//CONNECT JACK OUTPUT PORTS TO CLAM EXTERNGENERATORS
	const char ** portnames= jack_get_ports ( _jackClient , _jackOutPortAutoConnectList.c_str(), NULL, JackPortIsOutput);

	if (portnames==NULL)
	{
		std::cout << " -WARNING: couldn't locate any JACK output port <"
			<< _jackOutPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		int i=0;

		//Double iterate AudioSources & found JACK out ports
		for ( SourceJackBindings::iterator it= _sourceJackBindings.begin(); it!=_sourceJackBindings.end(); it++)
		{
			std::cout << "- Connecting " << portnames[i] << " -> " 
				<< it->PortName() << std::endl;

			if ( jack_connect( _jackClient, portnames[i], 
						it->PortName() ) !=0 )
			{
				std::cerr << " -WARNING: couldn't connect" << std::endl;
			}
		
			i++;
			if (portnames[i]==NULL) break;
		}	
	}		
	free(portnames);

	//CONNECT CLAM EXTERNSINKS TO JACK INPUT PORTS
	portnames= jack_get_ports ( _jackClient , _jackInPortAutoConnectList.c_str(), NULL, JackPortIsInput);

	if (portnames==NULL)
	{
		std::cout << " -WARNING: couldn't locate any JACK input port <"
			<< _jackInPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		int i=0;

		//Double iterate found JACK in ports & ExterSinks
		for (SinkJackBindings::iterator it= _sinkJackBindings.begin(); it!=_sinkJackBindings.end(); it++)
		{
			std::cout << "- Connecting "<< it->PortName()
				<< " -> " << portnames[i] << std::endl;

			if ( jack_connect( _jackClient, it->PortName(),
						portnames[i]) != 0)
			{
				std::cerr << " -WARNING: couldn't connect" << std::endl;
			}
		
			i++;
			if (portnames[i]==NULL) break;
		}
	}			
	free(portnames);
}



} //namespace CLAM


