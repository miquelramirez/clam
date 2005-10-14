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

//C++ version 
static char *dupstr( char const *args )
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
	unsigned long mClamBufferSize, mExternBufferSize;
	
public:
	NetworkLADSPAPlugin();
	~NetworkLADSPAPlugin();
	
	void Activate();
	void Deactivate();
	
	int GetPortCount()
	{
		return ( mReceiverList.size()+mSenderList.size() );
	}
	
	int GetControlCount()
	{
		return ( mInControlList.size() );
	}

	Network& GetNetwork()
	{
		return *mNet;
	}


	void ProcessInputPorts();
	void ProcessOutputPorts();
	void ProcessControls();

	void UpdatePortFrameAndHopSize();
	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);
	
	void Run( unsigned long nsamples );
	void CopyLadspaBuffersToGenerators(const unsigned long nframes);
	void CopySinksToLadspaBuffers(const unsigned long nframes);
	void ProcessControlValues();
};

} //namespace CLAM


#endif
