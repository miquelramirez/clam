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
#include "ExternInControl.hxx"

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
void ConnectTo(LADSPA_Handle, unsigned long, LADSPA_Data *);

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

typedef struct
{
	std::string controlName;
	ExternInControl *clamControlReceiver;
	LADSPA_Data *dataBuffer;
} LADSPAInControlInfo;

		
typedef std::vector<LADSPAInPortInfo> LADSPAInPortList;
typedef std::vector<LADSPAOutPortInfo> LADSPAOutPortList;
typedef std::vector<LADSPAInControlInfo> LADSPAInControlList;

class NetworkLADSPAPlugin
{
private:
	Network* mNet;		
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
	LADSPAInControlList mInControlList;
	
public:
	NetworkLADSPAPlugin()
	{
		mNet=new Network();
		GetNetwork().AddFlowControl( new PushFlowControl(512) );
		GetNetwork().SetName("Testing name");

		std::cerr << " constructor" << std::endl;

		char* xmlfile=getenv("CLAM_NETWORK_PLUGIN_PATH");
		if (xmlfile==NULL)
		{
			std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: no network file specified. Plugin not loaded" << std::endl;
			std::cerr << "                     --> Do 'export CLAM_NETWORK_PLUGIN_PATH=/..path../file.xml'" << std::endl;
			return;
		}
		
		try
		{
			XmlStorage::Restore( GetNetwork(), xmlfile);	//"genwire.xml");	
		}
		catch ( XmlStorageErr err)
		{
			std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: error opening file <" << xmlfile << "> . Plugin not loaded" <<std::endl;
			return;
		}
		
		std::cerr << "GET NAME="<< GetNetwork().GetName() << std::endl;
		
		std::cerr << " post-restore" << std::endl;

		GetInputPorts();
		GetOutputPorts();
		GetControls();
	}

	~NetworkLADSPAPlugin()
	{
		std::cerr << " destructor" << std::endl;
		delete mNet;
	}

	void Activate()
	{
		GetNetwork().Start();
	}
	
	void Deactivate()
	{
		GetNetwork().Stop();
	}
	
	void GetInputPorts()
	{
		CLAM_ASSERT( mReceiverList.empty(), "NetworkLADSPAPlugin::GetInputPorts() : there are already registered input ports");
	
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
				info.clamReceiver->SetFrameAndHopSize(1024); //TODO: cal obtenir el del host

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
				info.clamSender->SetFrameAndHopSize(1024);

				//Add the info 
				mSenderList.push_back(info);
			}
		}
	}
	
	void GetControls()
	{
		CLAM_ASSERT( mInControlList.empty(), "NetworkLADSPAPlugin::GetControls() : there are already registered controls");
	
		LADSPAInControlInfo info;
	
		//Get them from the Network and add it to local list		
		for (Network::ProcessingsMap::const_iterator it=GetNetwork().BeginProcessings(); it!=GetNetwork().EndProcessings(); it++)
		{
			if (std::string("ExternInControl")==std::string(it->second->GetClassName()))
			{
				//Store Processing name
				info.controlName=it->first;
				
				//Get Processing address
				info.clamControlReceiver=(ExternInControl*)it->second;

				//Add the info 
				mInControlList.push_back(info);
			}
		}
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


		//Manage InControls (ExternInControls)
		for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		{
			descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL);
			names[currentport] = dupstr( it->controlName.c_str() );

			//Obté processingConfig, i defineix cada param
			ExternInControlConfig& conf=const_cast<ExternInControlConfig&>(
							dynamic_cast<const ExternInControlConfig&>(
								it->clamControlReceiver->GetConfig() ));
			
			rangehints[currentport].LowerBound=(LADSPA_Data)conf.GetMinValue();
			rangehints[currentport].UpperBound=(LADSPA_Data)conf.GetMaxValue();
			rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE);
			currentport++;
				
		}
	}
	
	void Run( unsigned long nsamples )
	{
		ProcessControlValues();
		
		CopyLadspaBuffersToGenerators(nsamples);

		GetNetwork().Do();
		GetNetwork().Do();
		
		CopySinksToLadspaBuffers(nsamples);
	}

	void ProcessControlValues()
	{
		for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		{
			it->clamControlReceiver->Do( (float) *(it->dataBuffer) );
		}
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
	
	void ConnectTo(unsigned long port, LADSPA_Data * data)
	{
		
		if ( port <= mReceiverList.size()-1 ) //Input port
			mReceiverList.at( port ).dataBuffer=data;
		else if ( port <= mReceiverList.size() + mSenderList.size() -1) //Output port
			mSenderList.at( port-mReceiverList.size() ).dataBuffer=data;
		else //Input control
			mInControlList.at( port-mReceiverList.size()-mSenderList.size() ).dataBuffer=data;
	}
	
	int GetPortCount()
	{
		return ( mReceiverList.size()+mSenderList.size() );
	}
	
	int GetControlCount()
	{
		return ( mInControlList.size() );
	}
};

} //namespace CLAM


#endif
