#ifndef _NETWORK_LADSPA_PLUGIN_
#define _NETWORK_LADSPA_PLUGIN_

#include <ladspa.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include <fstream>
/*
 * COSES A FER:
 *  -Subdividir la construcció del descriptor
 *  
 */

//C++ version: choose 
char *dupstr( char const *args )
{
	char * s;
	size_t v;

	try
	{
		s = new char[v = strlen(args) + 1];
		memcpy( s, args, v );
	}
	catch( std::bad_alloc )
	{
		s = NULL;
	}
	return s;
} 

const LADSPA_Descriptor * ladspa_descriptor(unsigned long);
LADSPA_Handle Instantiate(const LADSPA_Descriptor *, unsigned long);
void Run(LADSPA_Handle, unsigned long);
void CleanUp(LADSPA_Handle);
void Activate(LADSPA_Handle);
void Deactivate(LADSPA_Handle);
void ConnectPortTo(LADSPA_Handle, unsigned long, LADSPA_Data *);

namespace CLAM
{
	class NetworkLADSPAPlugin;
}

class StartupShutdownHandler
{
private:
	void CreateLADSPADescriptor();
public:
	StartupShutdownHandler();
	~StartupShutdownHandler();
	CLAM::NetworkLADSPAPlugin * mPluginInstance;
};

namespace CLAM
{

typedef struct
{
	std::string portName;
	ExternGenerator *clamReceiver;
	LADSPA_Data *dataBuffer;
} LADSPAInPortInfo;

typedef struct
{
	std::string portName;
	ExternSink *clamSender;
	LADSPA_Data *dataBuffer;
} LADSPAOutPortInfo;

typedef std::vector<LADSPAInPortInfo> LADSPAInPortList;
typedef std::vector<LADSPAOutPortInfo> LADSPAOutPortList;

class NetworkLADSPAPlugin
{
private:
	Network* mNet;		
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
	
public:
	NetworkLADSPAPlugin()
	{
		mNet=new Network();
		GetNetwork().AddFlowControl( new PushFlowControl(512) );
		GetNetwork().SetName("Testing name");

		std::cerr << " constructor" << std::endl;

		try{
		XmlStorage::Restore( GetNetwork(), "SMSMessJACK.xml");
		}catch ( XmlStorageErr err)
		{

			std::cerr << " error!: " << err.what() << std::endl;

		}
		
		// ALTERNATIVE: initialize network manually
		//mNet->AddProcessing("input1", "ExternGenerator" );
		//mNet->AddProcessing("input2", "ExternGenerator" );
		//mNet->AddProcessing("output1", "ExternSink" );
		//mNet->AddProcessing("output2", "ExternSink" );
		//mNet->AddProcessing("input1", new ExternGenerator() );
		//mNet->AddProcessing("input2", new ExternGenerator() );
		//mNet->AddProcessing("output1", new ExternSink() );
		//mNet->AddProcessing("output2", new ExternSink() );
		//mNet->ConnectPorts("input1.AudioOut","output1.AudioIn");
		//mNet->ConnectPorts("input2.AudioOut","output2.AudioIn");
	
		std::cerr << "GET NAME="<< GetNetwork().GetName() << std::endl;
		
		std::cerr << " post-restore" << std::endl;

		GetInputPorts();
		GetOutputPorts();
		//In a future: GetInputControls();
	}

	void GetInputPorts()
	{
		CLAM_ASSERT( mReceiverList.empty(), "NetworkLADSPAPlugin::RegisterInputPorts() : there are already registered input ports");
	
		LADSPAInPortInfo info;
	
		//Get them from the Network and add it to local list		
		for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
		{
			if (std::string("ExternGenerator")==std::string(it->second->GetClassName()))
			{
				//Store Processing name
				info.portName=it->first;
				
				//Get Processing address
				info.clamReceiver=(ExternGenerator*)it->second;
				//NO!! info.clamReceiver->SetFrameAndHopSize(mJackBufferSize);

				//Add the info 
				mReceiverList.push_back(info);
			}
		}
	}
	
	void GetOutputPorts()
	{
		CLAM_ASSERT( mSenderList.empty(), "NetworkLADSPAPlugin::GetInputPorts() : there are already registered output ports");
	
		LADSPAOutPortInfo info;
	
		//Get them from the Network and add it to local list		
		for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
		{
			if (std::string("ExternSink")==std::string(it->second->GetClassName()))
			{
				//Store Processing name
				info.portName=it->first;
				
				//Get Processing address
				info.clamSender=(ExternSink*)it->second;
				//NO!! info.clamSender->SetFrameAndHopSize(mJackBufferSize);

				//Add the info 
				mSenderList.push_back(info);
			}
		}
	}
	
	~NetworkLADSPAPlugin()
	{
		std::cerr << " destructor" << std::endl;
		delete mNet;
	}

	Network& GetNetwork()
	{
		return *mNet;
	}

	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
	{
		std::cerr << " FILLPORTINFO" << std::endl;
		int currentport=0;

		//Manage InPorts (ExternGenerators)
		for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		{
			descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
			names[currentport] = dupstr( it->portName.c_str() );
			rangehints[currentport].HintDescriptor = 0;
			currentport++;
		}

		//Manage OutPorts (ExternSinks)
		for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		{
			descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
			names[currentport] = dupstr( it->portName.c_str() );
			rangehints[currentport].HintDescriptor = 0;
			currentport++;
		}
	}
	
	void Run( unsigned long nsamples )
	{
		CopyLadspaBuffersToGenerators(nsamples);

		GetNetwork().Do();
		
		CopySinksToLadspaBuffers(nsamples);
	}

	void CopyLadspaBuffersToGenerators(const unsigned long nframes)
	{
		for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		{
			it->clamReceiver->Do( (TData*) it->dataBuffer, nframes );
		}
	}
	
	void CopySinksToLadspaBuffers(const unsigned long nframes)
	{
		for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		{
			it->clamSender->Do( (TData*) it->dataBuffer, nframes);	
		}
	}
	
	void ConnectPortTo(unsigned long port, LADSPA_Data * data)
	{
		if ( port <= mReceiverList.size()-1 )
			mReceiverList.at( port ).dataBuffer=data;
		else
			mSenderList.at( port-mReceiverList.size() ).dataBuffer=data;
	}
	
	int GetPortCount()
	{
		return ( mReceiverList.size()+mSenderList.size() );
	}
};

} //namespace CLAM


#endif
