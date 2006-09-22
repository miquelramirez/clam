
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
	mAutoConnect=false;
	InitClient();
}

JACKNetworkPlayer::~JACKNetworkPlayer()
{
	Stop();
	
	//JACK CODE
	if ( jack_client_close (mJackClient) )
	{
		std::cerr << "JACK ERROR: cannot close client" << std::endl;
		exit(1);
	}
}

void JACKNetworkPlayer::InitClient()
{
	NotifyModification();

	//JACK CODE: init client
	
	mJackClientname="CLAM_client";		
	if ((mJackClient = jack_client_new ( mJackClientname.c_str() )) == 0)
	{
		std::cerr << "JACK ERROR: server not running?"<< std::endl;
		exit(1);
	}
	
	//Register callback method for processing
	if ( jack_set_process_callback (mJackClient, JackProcessingCallback, this) )
	{
		std::cerr << "JACK ERROR: registering process callbacks"<< std::endl;
		exit(1);
	}
	
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
	CLAM_ASSERT( mReceiverList.empty(), "JACKNetworkPlayer::RegisterInputPorts() : there are already registered input ports");
	
	JACKOutPortCouple pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "ExternGenerator") continue;

		//Store Processing name
		pair.portName=mJackClientname+std::string(":")+it->first;
		
		//Get Processing address
		pair.clamReceiver=(ExternGenerator*)it->second;
		pair.clamReceiver->SetFrameAndHopSize(mJackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (mJackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

		pair.connectedTo=NULL;
		
		//Add the pair (jack port, clam jack receiver) to the list
		mReceiverList.push_back(pair);
	}
}

void JACKNetworkPlayer::RegisterOutputPorts(const Network& net)
{
	CLAM_ASSERT( mSenderList.empty(), "JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");
	
	JACKInPortCouple pair;
	
	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
	{
		std::string processingClass = it->second->GetClassName();
		if (processingClass != "ExternSink") continue;

		//Store Processing name
		pair.portName=mJackClientname+":"+it->first;

		//Get Processing address
		pair.clamSender=(ExternSink*)it->second;
		pair.clamSender->SetFrameAndHopSize(mJackBufferSize);

		//Register port on the JACK server
		const std::string & processingName = it->first;
		pair.jackPort=jack_port_register (mJackClient,
			processingName.c_str(),
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

		pair.connectedTo=NULL;

		//Add the pair (jack port, clam jack receiver) to the list
		mSenderList.push_back(pair);
	}
}

void JACKNetworkPlayer::UnRegisterPorts()
{
	for (JACKInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		if ( jack_port_unregister ( mJackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->portName << std::endl;
			exit(1);
		}
	}
	mSenderList.clear();
	
	for (JACKOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		if ( jack_port_unregister ( mJackClient, it->jackPort) )
		{
			std::cerr << "JACK ERROR: unregistering port " << it->portName << std::endl;
			exit(1);
		}
	}
	mReceiverList.clear();
}

void JACKNetworkPlayer::CopyJackBuffersToGenerators(const jack_nframes_t nframes)
{
	for (JACKOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackInBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the ExternGenerator to put JACK's buffer info into CLAM
		(*it).clamReceiver->Do( (TData*)jackInBuffer, nframes );

	}

}

void JACKNetworkPlayer::CopySinksToJackBuffers(const jack_nframes_t nframes)
{
	for (JACKInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		//Retrieve JACK buffer location
		jack_default_audio_sample_t *jackOutBuffer = 
			(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackPort, nframes);
		//Tell the ExternGenerator to put CLAM's buffer info JACK
		it->clamSender->Do( (TData*)jackOutBuffer, nframes);	
	}
}

void JACKNetworkPlayer::Start()
{
	if (!IsStopped()) 
		return;
	
	SetStopped(false);
	
	if (IsModified())
	{
		UnRegisterPorts();
		RegisterPorts();
	}
	
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
	if (IsStopped())
		return;
	
	SetStopped(true);

	StoreConnections();
	
	//JACK CODE (the init order of network, ... should be decided)
	if ( jack_deactivate (mJackClient) )
	{
		std::cerr << "JACK ERROR: cannot deactivate client" << std::endl;
		exit(1);
	}

	GetNetwork().Stop();
}

void JACKNetworkPlayer::Do(const jack_nframes_t nframes)
{
	if (IsStopped())
		return;

	CopyJackBuffersToGenerators(nframes);
	
	for (int stepcount=0; stepcount < (int(nframes)/int(mClamBufferSize)); stepcount++)
		GetNetwork().Do();

	CopySinksToJackBuffers(nframes);
}

//Saves the connections made to our local in/out jack ports
void JACKNetworkPlayer::StoreConnections()
{
	for (JACKOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		const char** con = jack_port_get_connections ( it->jackPort );
		it->connectedTo=con;

		JackConnection connection;
		connection.processingName = it->portName;
		connection.outsideConnections = jack_port_get_connections ( it->jackPort );
		mIncomingJackConnections.push_back(connection);
	}
	
	for (JACKInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		const char** con = jack_port_get_connections ( it->jackPort );
		it->connectedTo=con;

		JackConnection connection;
		connection.processingName = it->portName;
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

		//Double iterate ExternGenerators & found JACK out ports
		for ( JACKOutPortList::iterator it= mReceiverList.begin(); it!=mReceiverList.end(); it++)
		{
			std::cout << "- Connecting " << portnames[i] << " -> " 
				<< it->portName << std::endl;

			if ( jack_connect( mJackClient, portnames[i], 
						it->portName.c_str() ) !=0 )
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
		for (JACKInPortList::iterator it= mSenderList.begin(); it!=mSenderList.end(); it++)
		{
			std::cout << "- Connecting "<< it->portName
				<< " -> " << portnames[i] << std::endl;

			if ( jack_connect( mJackClient, it->portName.c_str(),
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

