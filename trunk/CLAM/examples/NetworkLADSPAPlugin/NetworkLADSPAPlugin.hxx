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
#include "Utils.hxx"

/*
 * COSES A FER:
 *  -Subdividir la construcció del descriptor
 *  -Crear/plagiar de JACKProt els mètodes per analitzar Externalizers i desar-ne la info
 *  -Aplicar aquella info a la construcció del descriptor
 *  -Problemes amb els strdups?!
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
//Alternative version to strdup because it is not ANSI standard, so we save problems
char *dupstrC(const char *s)
{
	char *p = (char*) malloc( strlen(s) + 1);
	if ( p != NULL ) strcpy( p, s);
	else
	{
		std::cerr << "ERROR: allocating in dupstr" << std::endl;
		exit(-1);
	}
	return p;
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

		std::cerr << " constructor" << std::endl;

		XmlStorage::Restore( GetNetwork(), "wire.xml");
		
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

				std::cerr << " externgenerator!" << std::endl;

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
				
				std::cerr << " externsink!" << std::endl;
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

		descriptors[0] = ( LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO );
		descriptors[1] = ( LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO );
		
		names[0] = dupstr("Input");
		names[1] = dupstr("Output");
	
		rangehints[0].HintDescriptor = 0;
		rangehints[1].HintDescriptor = 0;

	}
	
	unsigned int GetPortCount()
	{
		return ( mReceiverList.size()+mSenderList.size() );
	}
	LADSPA_Data * mInput, *mOutput;
};

} //namespace CLAM


#endif
