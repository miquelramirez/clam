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
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "LadspaLibrary.hxx"

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

class NetworkLADSPAPlugin
{
private:
	typedef std::vector< LADSPAInfo<AudioSource> > LADSPAInPortList;
	typedef std::vector< LADSPAInfo<AudioSink> > LADSPAOutPortList;
	typedef std::vector< LADSPAInfo<ControlSource> > LADSPAInControlList;
	typedef std::vector< LADSPAInfo<ControlSink> > LADSPAOutControlList;

	Network _network;
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
	LADSPAInControlList mInControlList;
	LADSPAOutControlList mOutControlList;
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
		return ( mInControlList.size()+mOutControlList.size() );
	}

	Network& GetNetwork()
	{
		return _network;
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
	static void DestructDescriptor(LADSPA_Descriptor * descriptor);
	static LADSPA_Descriptor * CreateLADSPADescriptor();
};

class LadspaNetworkExporter
{
public:
	LadspaNetworkExporter(LadspaLibrary & library)
	{
		LADSPA_Descriptor * descriptor = NetworkLADSPAPlugin::CreateLADSPADescriptor();
		library.AddPluginType(descriptor);
	}
};

} //namespace CLAM


#endif

