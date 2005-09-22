
#ifndef _JACK_NETWORK_PLAYER_HXX_
#define _JACK_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include "BasicFlowControl.hxx"

////////JACK CODE
#include <jack/jack.h>

namespace CLAM
{

int JackLoopCallback (jack_nframes_t, void *);
void JackShutdownCallback (void *);

typedef struct
{
	std::string portName;
	jack_port_t* jackOutPort;
	CLAM::ExternGenerator* clamReceiver;
} JACKOutPortCouple;

typedef struct
{
	std::string portName;
	jack_port_t* jackInPort;
	CLAM::ExternSink* clamSender;
} JACKInPortCouple;

typedef std::vector<JACKOutPortCouple> JACKOutPortList;
typedef std::vector<JACKInPortCouple> JACKInPortList;

class JACKNetworkPlayer
{
	CLAM::Network *mNetwork;	

	int mJackSampleRate, mJackBufferSize, mClamBufferSize;
	bool mModified, mAutoConnect, mStopped;

	JACKOutPortList mReceiverList;
	JACKInPortList mSenderList;
	std::string mJackOutPortAutoConnectList, mJackInPortAutoConnectList;
	
	//JACK CODE
	jack_client_t * mJackClient;
	std::string mJackClientname;

public:
	//Called by the prototyper
	JACKNetworkPlayer(const std::string & networkFile, std::list<std::string> portlist)
	{
		CLAM::PushFlowControl * fc = new CLAM::PushFlowControl(mClamBufferSize);
		GetNetwork().AddFlowControl( fc );

		CLAM::XmlStorage::Restore(GetNetwork(), networkFile);
	
		mJackOutPortAutoConnectList=portlist.front();
		portlist.pop_front();
		mJackInPortAutoConnectList=portlist.front();

		//If neither input or output connections specified, we don't want any attempt of automatic connection
		if (mJackOutPortAutoConnectList==std::string("NULL") && mJackInPortAutoConnectList==std::string("NULL"))
			mAutoConnect=false;
		else
			mAutoConnect=true;


		InitClient();
	
	}

	JACKNetworkPlayer()
	{
		mAutoConnect=false;
		InitClient();
	}

	void InitClient()
	{
		mClamBufferSize=512;

		NotifyModification();
	
		//JACK CODE
		//init client
		
		mJackClientname="CLAM_client";		
		if ((mJackClient = jack_client_new ( mJackClientname.c_str() )) == 0)
		{
			std::cerr << "JACK ERROR: server not running?"<< std::endl;
			exit(1);
		}
		
		//Register callback method for processing
		if ( jack_set_process_callback (mJackClient, JackLoopCallback, this) )
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

	
	void NotifyModification()
	{
		mModified=true;	
	}

	void RegisterPorts()
	{
		RegisterInputPorts( GetNetwork() );
		RegisterOutputPorts( GetNetwork() );
		mModified=false;
	}
	
	void RegisterInputPorts(const CLAM::Network& net)
	{
		CLAM_ASSERT( mReceiverList.empty(), "JACKNetworkPlayer::RegisterInputPorts() : there are already registered input ports");
		
		JACKOutPortCouple pair;
		
		//Get them from the Network and add it to local list		
		for (CLAM::Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
		{
			if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
			{
				//Store Processing name
				pair.portName=mJackClientname+std::string(":")+it->first;
				
				//Get Processing address
				pair.clamReceiver=(CLAM::ExternGenerator*)it->second;
				pair.clamReceiver->SetFrameAndHopSize(mJackBufferSize);

				//Register port on the JACK server
				pair.jackOutPort=jack_port_register (mJackClient,
					it->first.c_str(),
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

				//Add the pair (jack port, clam jack receiver) to the list
				mReceiverList.push_back(pair);
			}
		}
	}
	
	void RegisterOutputPorts(const CLAM::Network& net)
	{
		CLAM_ASSERT( mSenderList.empty(), "JACKNetworkPlayer::RegisterOutputPorts() : there are already registered output ports");
		
		JACKInPortCouple pair;
		
		//Get them from the Network and add it to local list		
		for (CLAM::Network::ProcessingsMap::const_iterator it=net.BeginProcessings(); it!=net.EndProcessings(); it++)
		{
			if (std::string("ExternSink")==std::string(it->second->GetClassName()))
			{
				//Store Processing name
				pair.portName=mJackClientname+std::string(":")+it->first;

				//Get Processing address
				pair.clamSender=(CLAM::ExternSink*)it->second;
				pair.clamSender->SetFrameAndHopSize(mJackBufferSize);

				//Register port on the JACK server
				pair.jackInPort=jack_port_register (mJackClient,
					it->first.c_str(),
					JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

				//Add the pair (jack port, clam jack receiver) to the list
				mSenderList.push_back(pair);
			}
		}
	}

	void UnRegisterPorts()
	{
		for (JACKInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		{
			if ( jack_port_unregister ( mJackClient, it->jackInPort) )
			{
				std::cerr << "JACK ERROR: unregistering port " << it->portName << std::endl;
				exit(1);
			}
		}
		mSenderList.clear();
		
		for (JACKOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		{
			if ( jack_port_unregister ( mJackClient, it->jackOutPort) )
			{
				std::cerr << "JACK ERROR: unregistering port " << it->portName << std::endl;
				exit(1);
			}
		}
		mReceiverList.clear();
	}

	void CopyJackBuffersToGenerators(const jack_nframes_t nframes)
	{
		for (JACKOutPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackInBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackOutPort, nframes);
			//Tell the ExternGenerator to put JACK's buffer info into CLAM
			(*it).clamReceiver->Do( (CLAM::TData*)jackInBuffer, nframes );
	
		}

	}
	
	void CopySinksToJackBuffers(const jack_nframes_t nframes)
	{
		for (JACKInPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		{
			//Retrieve JACK buffer location
			jack_default_audio_sample_t *jackOutBuffer = 
				(jack_default_audio_sample_t*) jack_port_get_buffer ( it->jackInPort, nframes);
			//Tell the ExternGenerator to put CLAM's buffer info JACK
			(*it).clamSender->Do( (CLAM::TData*)jackOutBuffer, nframes);	
		}
	}

	void Do(const jack_nframes_t nframes)
	{
		if (mStopped)
			return;

		CopyJackBuffersToGenerators(nframes);
		
		for (int stepcount=0; stepcount < (int(nframes)/int(mClamBufferSize)); stepcount++)
			GetNetwork().Do();

		CopySinksToJackBuffers(nframes);
	}

	
	void Start()
	{
		if (!mStopped) 
			return;
		
		mStopped=false;
		
		if (mModified)
		{
			UnRegisterPorts();
			RegisterPorts();
		}
		
		//GetNetwork().ReconfigureAllProcessings();
		GetNetwork().Start();

		//JACK CODE (the init order of network, ... should be decided)
		if (jack_activate (mJackClient)) {
			std::cerr << "JACK ERROR: cannot activate client" << std::endl;
			exit(1);
		}

		if (mAutoConnect)
			AutoConnectPorts();
	}
	
	void Stop()
	{
		if (mStopped)
			return;
		
		mStopped=true;
		
		//JACK CODE (the init order of network, ... should be decided)
		if ( jack_deactivate (mJackClient) )
		{
			std::cerr << "JACK ERROR: cannot deactivate client" << std::endl;
			exit(1);
		}

		GetNetwork().Stop();
	}
	
	~JACKNetworkPlayer()
	{
		Stop();
		
		//JACK CODE
		if ( jack_client_close (mJackClient) )
		{
			std::cerr << "JACK ERROR: cannot close client" << std::endl;
			exit(1);
		}
	}
	
	CLAM::Network & Network()
	{
		return *mNetwork;
	}
	
	void AutoConnectPorts()
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

	void SetNetwork (CLAM::Network& net)
	{
		mNetwork=&net;
		NotifyModification();
	}

	CLAM::Network& GetNetwork()
	{
		CLAM_ASSERT( (mNetwork!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
		return *mNetwork;
	}
};

//JACK CODE
inline int JackLoopCallback (jack_nframes_t nframes, void *arg)
{
	CLAM::JACKNetworkPlayer* player=(CLAM::JACKNetworkPlayer*)arg;
	player->Do(nframes);

	return 0;
}

inline void JackShutdownCallback (void *arg)
{
	CLAM::JACKNetworkPlayer* player=(CLAM::JACKNetworkPlayer*)arg;
	delete player;
}

} //namespace CLAM

#endif
