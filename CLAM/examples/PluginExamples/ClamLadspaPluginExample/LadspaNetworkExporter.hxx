#ifndef LadspaNetworkExporter_hxx
#define LadspaNetworkExporter_hxx

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

/// Include a given file as binary data
/// @arg varname is the variable that will hold the var (const char[])
/// @arg file is an absolute path or a relative path from the build point (not the source file!!)
/// @todo Try to make CLAM_EXTERNAL_FILE_DATA work with path relatives to the source that uses the macro.
#define CLAM_EXTERNAL_FILE_DATA(varname, file) \
asm ( \
"  .section \".rodata\"\n" \
"   .type " #varname ", @object\n" \
" " #varname ":\n" \
"   .incbin \"" file "\"\n" \
"   .byte 0\n" \
"   .size " #varname ", .-" #varname "\n" \
); \
extern const char varname[]; \

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
	NetworkLADSPAPlugin(const std::string & networkXmlContent);
	~NetworkLADSPAPlugin();
	
	void Activate();
	void Deactivate();
	
	void LocateConnections();
	void UpdatePortFrameAndHopSize();
	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);
	
	void Run( unsigned long nsamples );
	void CopyLadspaBuffersToGenerators(const unsigned long nframes);
	void CopySinksToLadspaBuffers(const unsigned long nframes);
	void ProcessInControlValues();
	void ProcessOutControlValues();
	static LADSPA_Descriptor * CreateLADSPADescriptor(
		const std::string & networkXmlContent,
		unsigned id,
		const std::string & label,
		const std::string & name,
		const std::string & maker,
		const std::string & copyright
	);
};

class LadspaNetworkExporter
{
public:
	LadspaNetworkExporter(LadspaLibrary & library, const std::string & networkXmlContent,
		unsigned id,
		const std::string & label,
		const std::string & name,
		const std::string & maker,
		const std::string & copyright
		)
	{
		LADSPA_Descriptor * descriptor = NetworkLADSPAPlugin::CreateLADSPADescriptor(
			networkXmlContent, id, label, name,
			maker, copyright);
		if (not descriptor) return;
		library.AddPluginType(descriptor);
	}
};

} //namespace CLAM


#endif//LadspaNetworkExporter_hxx

