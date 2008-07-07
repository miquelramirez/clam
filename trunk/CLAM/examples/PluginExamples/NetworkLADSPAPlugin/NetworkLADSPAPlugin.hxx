#ifndef _NETWORK_LADSPA_PLUGIN_
#define _NETWORK_LADSPA_PLUGIN_

#include <ladspa.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"
//#include "ExternInControl.hxx" // TODO: Port
//#include "ExternOutControl.hxx" // TODO: Port

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

template<class T>
class LADSPAInfo
{
public:
	std::string name;
	T* processing;
	LADSPA_Data *dataBuffer;
};

typedef std::vector< LADSPAInfo<AudioSource> > LADSPAInPortList;
typedef std::vector< LADSPAInfo<AudioSink> > LADSPAOutPortList;
//typedef std::vector< LADSPAInfo<ExternInControl> > LADSPAInControlList; // TODO: Port
//typedef std::vector< LADSPAInfo<ExternOutControl> > LADSPAOutControlList; // TODO: Port

class NetworkLADSPAPlugin
{
private:
	Network* mNet;		
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
//	LADSPAInControlList mInControlList; // TODO: Port
//	LADSPAOutControlList mOutControlList; // TODO: Port
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
		return 0;
//		return ( mInControlList.size()+mOutControlList.size() ); // TODO: Port
	}

	Network& GetNetwork()
	{
		return *mNet;
	}


	void ProcessInputPorts();
	void ProcessOutputPorts();
	void ProcessInputControls();
	void ProcessOutputControls();

	void UpdatePortFrameAndHopSize();
	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);
	
	void Run( unsigned long nsamples );
	void CopyLadspaBuffersToGenerators(const unsigned long nframes);
	void CopySinksToLadspaBuffers(const unsigned long nframes);
	void ProcessInControlValues();
	void ProcessOutControlValues();
};

} //namespace CLAM


#endif

