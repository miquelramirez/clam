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
	DYN_ATTRIBUTE( 0, public, std::string, Name ); 
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

typedef std::vector< LADSPAInfo<ExternInControl> > LADSPAInControlList;

class CLAMRemoteController
{
private:
	Network* mNet;		
	LADSPAInControlList mInControlList;
	LADSPAInfo<void> mPortList[4];
	std::vector<OSCSender*> mSenderList;
	TData* mLastValues;
		
	RemoteControlList mRemoteControlList;
	
public:
	CLAMRemoteController();
	~CLAMRemoteController();
	void CreateStructure();
	
	int GetPortCount()
	{
		return 4;
	}
	
	int GetControlCount()
	{
		return ( mInControlList.size() );
	}

	Network& GetNetwork()
	{
		return *mNet;
	}


	void ProcessPorts();

	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);
	
	void Run( unsigned long nsamples );
	void ProcessInControlValues();
	void CopyInPortsToOutputPorts( unsigned long nsamples );
};

} //namespace CLAM

#endif

