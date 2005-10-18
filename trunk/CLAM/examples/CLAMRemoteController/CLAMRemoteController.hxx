#ifndef _CLAM_REMOTE_CONTROLLER_
#define _CLAM_REMOTE_CONTROLLER_

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
#include "ExternOutControl.hxx"
#include "Text.hxx"
#include "OSCSender.hxx"

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

class RemoteControlPair : public CLAM::DynamicType
{
public:
	DYNAMIC_TYPE( RemoteControlPair, 3 );
	DYN_ATTRIBUTE( 0, public, Text, Name ); 
	DYN_ATTRIBUTE( 1, public, ExternInControlConfig, ControlConfig );
	DYN_ATTRIBUTE( 2, public, OSCSenderConfig, SenderConfig );

protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
};

class RemoteControlList : public CLAM::DynamicType
{
public:
	DYNAMIC_TYPE( RemoteControlList, 1 );
	DYN_CONTAINER_ATTRIBUTE( 0, public, std::list<RemoteControlPair>, List, Pair);
 
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
};

template<class T>
class LADSPAInfo
{
public:
	std::string name;
	T* processing;
	LADSPA_Data *dataBuffer;
};

typedef std::vector< LADSPAInfo<ExternGenerator> > LADSPAInPortList;
typedef std::vector< LADSPAInfo<ExternSink> > LADSPAOutPortList;
typedef std::vector< LADSPAInfo<ExternInControl> > LADSPAInControlList;
typedef std::vector< LADSPAInfo<ExternOutControl> > LADSPAOutControlList;

class CLAMRemoteController
{
private:
	Network* mNet;		
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
	LADSPAInControlList mInControlList;
	LADSPAOutControlList mOutControlList;
	unsigned long mClamBufferSize, mExternBufferSize;
	RemoteControlList mRemoteControlList;
	
public:
	CLAMRemoteController();
	~CLAMRemoteController();
	void CreateNetwork();
	
	void Activate();
	void Deactivate();
	
	int GetPortCount()
	{
		return ( mReceiverList.size()+mSenderList.size() );
	}
	
	int GetControlCount()
	{
		return ( mInControlList.size()+mOutControlList.size() );
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
