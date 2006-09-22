
#include "JACKNetworkPlayer.hxx"

#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

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
	// TODO: Do clearing jack stuff but do not delete the player!!!!!
	std::cout << "Shuting down jack client" << std::endl;
	delete player;
}

JACKNetworkPlayer::JACKNetworkPlayer(const std::string & networkFile, std::list<std::string> portlist)
{
	mClamBufferSize=512;

	SetNetwork( *( new Network() ) );
	GetNetwork().AddFlowControl( new PushFlowControl(mClamBufferSize) );

	XmlStorage::Restore(GetNetwork(), networkFile);

	mJackOutPortAutoConnectList=portlist.front();
	portlist.pop_front();
	mJackInPortAutoConnectList=portlist.front();

	//If neither input or output connections specified, we don't want any attempt of automatic connection
	if (mJackOutPortAutoConnectList=="NULL" && mJackInPortAutoConnectList==std::string("NULL"))
		mAutoConnect=false;
	else
		mAutoConnect=true;

	InitClient();

}

JACKNetworkPlayer::JACKNetworkPlayer()
{
	mClamBufferSize=512;
	mAutoConnect=false;
	InitClient();
}

JACKNetworkPlayer::~JACKNetworkPlayer()
{
	Stop();
	
	if (not mJackClient) return;
	bool error = jack_client_close (mJackClient);
	if (error)
	{
		std::cerr << "JACK ERROR: cannot close client" << std::endl;
		exit(1);
	}
}

bool JACKNetworkPlayer::IsConnectedToServer() const
{
	return mJackClient != 0;
}

void JACKNetworkPlayer::InitClient()
{
	NotifyModification();

	mJackClientname="CLAM_client";		
	mJackClient = jack_client_new ( mJackClientname.c_str() );
	if (not mJackClient)
	{
		std::cerr << "JACK ERROR: server not running?"<< std::endl;
		return;
	}
	
	//Register callback method for processing
	bool err = jack_set_process_callback (mJackClient, JackProcessingCallback, this);
	CLAM_ASSERT(not err, "JACK ERROR: registering process callbacks");
	
	//Register shutdown callback
	jack_on_shutdown (mJackClient, JackShutdownCallback, this);
	
	//Get JACK information
	mJackSampleRate=(int)jack_get_sample_rate (mJackClient);
	mJackBufferSize=(int)jack_get_buffer_size (mJackClient);
}


void JACKNetworkPlayer::RegisterPorts()
{
	RegisterInputPorts( GetNetwork() );
	RegisterOutputPorts( GetNetwork() );
	SetModified(false);
}

void JACKNetworkPlayer::RegisterInputPorts(const Network& net)
{
	CLAM_ASSERT( mSourceJackBindings.empty(), "JACKNetworkPlayer::RegisterInputPorts() : there are already registered input ports");
	
	SourceJackBinding pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "AudioSource") continue;

		//Get Processing address
		pair.source=(AudioSource*)it->second;
		pair.source->SetFrameAndHopSize(mJackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (mJackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		//Add the pair (jack port, clam jack receiver) to the list
		mSourceJackBindings.push_back(pair);
	}
}

void JACKNetworkPlayer::RegisterOutputPorts(const Network& net)
{
	CLAM_ASSERT( mSinkJackBindigs.empty(), "JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");
	
	SinkJackBinding pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "AudioSink") continue;

		//Get Processing address
		pair.sink=(AudioSink*)it->second;
		pair.sink->SetFrameAndHopSize(mJackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (mJackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

		//Add the pair (jack port, clam jack receiver) to the list
		mSinkJackBindigs.push_back(pair);
	}
}

void JACKNetworkPlayer::UnRegisterPorts()
{
	for (SinkJackBindings::iterator it=mSinkJackBindigs.begin(); it!=mSinkJackBindigs.end(); it++)
	{
		if ( jack_port_unregister ( mJackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	mSinkJackBindigs.clear();
	
	for (SourceJackBindings::iterator it=mSourceJackBindings.begin(); it!=mSourceJackBindings.end(); it++)
	{
		if ( jack_port_unregister ( mJackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->PortName() << std::endl;
			exit(1);
		}
	}
	mSourceJackBindings.clear();
}

void JACKNetworkPlayer::CopyJackBuffersToGenerators(const jack_nframes_t nframes)
{
	for (SourceJackBindings::iterator it=mSourceJackBindings.begin(); it!=mSourceJackBindings.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackInBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the AudioSource to put JACK's buffer info into CLAM
		it->source->Do( (TData*)jackInBuffer, nframes );

	}

}

void JACKNetworkPlayer::CopySinksToJackBuffers(const jack_nframes_t nframes)
{
	for (SinkJackBindings::iterator it=mSinkJackBindigs.begin(); it!=mSinkJackBindigs.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackOutBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the AudioSource to put CLAM's buffer info JACK
		it->sink->Do( (TData*)jackOutBuffer, nframes);	
	}
}

void JACKNetworkPlayer::Start()
{
	if (!IsStopped()) return;

	if (!mJackClient) return;
	
	SetStopped(false);
	
	UnRegisterPorts();
	RegisterPorts();
		
	GetNetwork().Start();

	//JACK CODE (the init order of network, ... should be decided)
	if (jack_activate (mJackClient)) {
		std::cerr << "JACK ERROR: cannot activate client" << std::endl;
		exit(1);
	}

	if (mAutoConnect)
		AutoConnectPorts();
	else
		RestoreConnections();
}

void JACKNetworkPlayer::Stop()
{
	if (IsStopped()) return;
	if (not mJackClient) return;
	
	SetStopped(true);

	StoreConnections();
	
	if ( jack_deactivate (mJackClient) )
	{
		std::cerr << "JACK ERROR: cannot deactivate client" << std::endl;
		exit(1);
	}

	GetNetwork().Stop();
}

void JACKNetworkPlayer::Do(const jack_nframes_t nframes)
{
	if (IsStopped()) return;

	CopyJackBuffersToGenerators(nframes);
	
	for (int stepcount=0; stepcount < (int(nframes)/int(mClamBufferSize)); stepcount++)
		GetNetwork().Do();

	CopySinksToJackBuffers(nframes);
}

//Saves the connections made to our local in/out jack ports
void JACKNetworkPlayer::StoreConnections()
{
	for (SourceJackBindings::iterator it=mSourceJackBindings.begin(); it!=mSourceJackBindings.end(); it++)
	{
		JackConnection connection;
		connection.processingName = it->PortName();
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
		mIncomingJackConnections.push_back(connection);
	}
	
	for (SinkJackBindings::iterator it=mSinkJackBindigs.begin(); it!=mSinkJackBindigs.end(); it++)
	{
		JackConnection connection;
		connection.processingName = it->PortName();
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
		mOutgoingJackConnections.push_back(connection);
	}
}

//Loads the connections made to our local in/out jack ports
void JACKNetworkPlayer::RestoreConnections()
{
	for (JackConnections::iterator it=mIncomingJackConnections.begin(); it!=mIncomingJackConnections.end(); it++)
	{
		if (not it->outsideConnections) continue;
		for (unsigned i=0; it->outsideConnections[i]; i++)
		{
			bool error = jack_connect ( mJackClient, it->outsideConnections[i], it->processingName.c_str() );
			if (error)
				std::cerr << "JACK WARNING: could not reconnect ports ( " <<
					it->processingName << " , " << it->outsideConnections[i] << " )" <<std::endl;
		}
		free(it->outsideConnections);
	}
	mIncomingJackConnections.clear();

	for (JackConnections::iterator it=mOutgoingJackConnections.begin(); it!=mOutgoingJackConnections.end(); it++)
	{
		if (not it->outsideConnections) continue;
		for (unsigned i=0; it->outsideConnections[i]; i++)
		{
			bool error = jack_connect ( mJackClient, it->processingName.c_str(), it->outsideConnections[i] );
			if (error)
				std::cerr << "JACK WARNING: could not reconnect ports ( " <<
					it->outsideConnections[i] << " , " << it->processingName << " )" <<std::endl;
		}
		free(it->outsideConnections);
	}
	mOutgoingJackConnections.clear();
}

void JACKNetworkPlayer::AutoConnectPorts()
{
	//Automatically connect the ports to external jack ports
	std::cout << "Automatically connecting to JACK input and output ports" << std::endl;
	
	//CONNECT JACK OUTPUT PORTS TO CLAM EXTERNGENERATORS
	const char ** portnames= jack_get_ports ( mJackClient , mJackOutPortAutoConnectList.c_str(), NULL, JackPortIsOutput);

	if (portnames==NULL)
	{
		std::cout << " -WARNING: couldn't locate any JACK output port <"
			<< mJackOutPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		int i=0;

		//Double iterate AudioSources & found JACK out ports
		for ( SourceJackBindings::iterator it= mSourceJackBindings.begin(); it!=mSourceJackBindings.end(); it++)
		{
			std::cout << "- Connecting " << portnames[i] << " -> " 
				<< it->PortName() << std::endl;

			if ( jack_connect( mJackClient, portnames[i], 
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
	portnames= jack_get_ports ( mJackClient , mJackInPortAutoConnectList.c_str(), NULL, JackPortIsInput);

	if (portnames==NULL)
	{
		std::cout << " -WARNING: couldn't locate any JACK input port <"
			<< mJackInPortAutoConnectList << ">"<<std::endl;
	}
	else
	{
		int i=0;

		//Double iterate found JACK in ports & ExterSinks
		for (SinkJackBindings::iterator it= mSinkJackBindigs.begin(); it!=mSinkJackBindigs.end(); it++)
		{
			std::cout << "- Connecting "<< it->PortName()
				<< " -> " << portnames[i] << std::endl;

			if ( jack_connect( mJackClient, it->PortName(),
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


