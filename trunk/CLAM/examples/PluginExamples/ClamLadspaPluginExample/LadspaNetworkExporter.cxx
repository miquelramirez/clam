#include "LadspaNetworkExporter.hxx"

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

CLAM_EXTERNAL_FILE_DATA(embededNetwork, "PluginExamples/ClamLadspaPluginExample/genderChange.clamnetwork")

// Ladspa Callbacks
extern "C"
{
	// Construct a new plugin instance.
	static LADSPA_Handle Instantiate(const LADSPA_Descriptor * descriptor, unsigned long sampleRate)
	{
		std::cerr << "Network2Ladspa: instantiate" << std::endl;
		return new CLAM::NetworkLADSPAPlugin();
	}
	// Destruct plugin instance
	static void CleanUp(LADSPA_Handle handle)
	{
		std::cerr << "Network2Ladspa: cleanup " << handle << std::endl;
		delete (CLAM::NetworkLADSPAPlugin*) handle;
	}

	// Run the plugin
	static void Run(LADSPA_Handle handle, unsigned long sampleCount)
	{
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->Run( sampleCount );
	}
	// Activate Plugin
	static void Activate(LADSPA_Handle handle)
	{
		std::cerr << "Network2Ladspa: activate " << handle << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->Activate();
	}

	static void Deactivate(LADSPA_Handle handle)
	{
		std::cerr << "Network2Ladspa: deactivate " << handle << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->Deactivate();
	}

	// Connect a port to a data location.
	static void ConnectTo(LADSPA_Handle handle, unsigned long port, LADSPA_Data * dataLocation)
	{
		std::cerr << "Network2Ladspa: connect " << port << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->ConnectTo( port, dataLocation );
	}
}

namespace CLAM
{

NetworkLADSPAPlugin::NetworkLADSPAPlugin()
{
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;
	_network.SetName("Testing name");

	std::cerr << " constructor" << std::endl;
/*
	const char* xmlfile=getenv("CLAM_NETWORK_PLUGIN_PATH");
	if (xmlfile==NULL)
	{
		std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: no network file specified. Plugin not loaded" << std::endl;
		std::cerr << "                    --> Do \"export CLAM_NETWORK_PLUGIN_PATH=/..path../file.xml\"" << std::endl;
		return;
	}
*/
	std::istringstream xmlfile(embededNetwork);
	try
	{
		XmlStorage::Restore( _network, xmlfile);
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAM::NetworkLADSPAPlugin WARNING: error opening file <" << xmlfile << "> . Plugin not loaded" <<std::endl;
		std::cerr << err.what() << std::endl;
		return;
	}

	LocateConnections();
}

NetworkLADSPAPlugin::~NetworkLADSPAPlugin()
{
	std::cerr << "NetworkLADSPAPlugin: DELETED" << std::endl;
}

void NetworkLADSPAPlugin::Activate()
{
	_network.Start();
}

void NetworkLADSPAPlugin::Deactivate()
{
	_network.Stop();
}

void NetworkLADSPAPlugin::LocateConnections()
{
	CLAM_ASSERT( mReceiverList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered input ports");
	CLAM_ASSERT( mSenderList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered output ports");
	CLAM_ASSERT( mInControlList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered controls");
	CLAM_ASSERT( mOutControlList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered controls");

	//Get them from the Network and add it to local list		
	for (Network::ProcessingsMap::const_iterator it=_network.BeginProcessings(); it!=_network.EndProcessings(); it++)
	{
		CLAM::Processing * processing = it->second;
		const std::string & className = processing->GetClassName();
		if (className == "AudioSource")
		{
			LADSPAInfo<AudioSource> info;
			info.name=it->first;
			info.processing=(AudioSource*)processing;
			info.processing->SetFrameAndHopSize( mExternBufferSize );
			mReceiverList.push_back(info);
		}
		if (className == "AudioSink")
		{
			LADSPAInfo<AudioSink> info;
			info.name=it->first;
			info.processing=(AudioSink*)it->second;
			info.processing->SetFrameAndHopSize( mExternBufferSize );
			mSenderList.push_back(info);
		}
		if (className == "ControlSource")
		{
			LADSPAInfo<ControlSource> info;
			info.name=it->first;
			info.processing=(ControlSource*)it->second;
			mInControlList.push_back(info);
		}
		if (className == "ControlSink")
		{
			LADSPAInfo<ControlSink> info;
			info.name=it->first;
			info.processing=(ControlSink*)it->second;
			mOutControlList.push_back(info);
		}
	}
}

void NetworkLADSPAPlugin::UpdatePortFrameAndHopSize()
{
	//AudioSources
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );

	//AudioSinks
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetFrameAndHopSize( mExternBufferSize );
}

void NetworkLADSPAPlugin::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
{
	int currentport=0;

	//Manage InPorts (AudioSources)
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
		names[currentport] = LadspaLibrary::dupstr( it->name.c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage OutPorts (AudioSinks)
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
		names[currentport] = LadspaLibrary::dupstr( it->name.c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage InControls (ExternInControls)
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL);
		names[currentport] = LadspaLibrary::dupstr( it->name.c_str() );

		//Obt� processingConfig, i defineix cada param
		ControlSourceConfig conf = dynamic_cast<const ControlSourceConfig&>(it->processing->GetConfig() );
		
		rangehints[currentport].LowerBound=(LADSPA_Data)conf.GetMinValue();
		rangehints[currentport].UpperBound=(LADSPA_Data)conf.GetMaxValue();
		rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE);
		currentport++;
	}
	
	//Manage OutControls (ExternOutControls)
	// (Please note that not all the LADSPA hosts make use of these kind of ports)
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL);
		names[currentport] = LadspaLibrary::dupstr( it->name.c_str() );
		rangehints[currentport].LowerBound=(LADSPA_Data)0;
		rangehints[currentport].UpperBound=(LADSPA_Data)1000;
		rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE);
		currentport++;
	}
}

void NetworkLADSPAPlugin::Run( unsigned long nsamples )
{
	//Check current buffer size of ports, to make sure everything fits!
	// if it isn't so, upgrade Frame and Hop sizes (vital)
	if (nsamples!=mExternBufferSize)
	{
		mExternBufferSize=nsamples;
		UpdatePortFrameAndHopSize();
	}		
	
	ProcessInControlValues();
	
	CopyLadspaBuffersToGenerators(nsamples);

	//Do() as much as it is needed
	for (int stepcount=0; stepcount < (int(mExternBufferSize)/int(mClamBufferSize)); stepcount++)
		_network.Do();

	CopySinksToLadspaBuffers(nsamples);
	ProcessOutControlValues();
}

void NetworkLADSPAPlugin::ProcessInControlValues()
{
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		it->processing->Do( (float) *(it->dataBuffer) );
}

void NetworkLADSPAPlugin::ProcessOutControlValues()
{
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
		*(it->dataBuffer)=it->processing->GetControlValue();
}

void NetworkLADSPAPlugin::CopyLadspaBuffersToGenerators(const unsigned long nframes)
{
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
		it->processing->SetExternalBuffer(it->dataBuffer, nframes );
}

void NetworkLADSPAPlugin::CopySinksToLadspaBuffers(const unsigned long nframes)
{
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
		it->processing->SetExternalBuffer(it->dataBuffer, nframes );
}

void NetworkLADSPAPlugin::ConnectTo(unsigned long port, LADSPA_Data * data)
{
	if ( port <= mReceiverList.size()-1 ) //Input port
		mReceiverList.at( port ).dataBuffer=data;
	else if ( port <= mReceiverList.size() + mSenderList.size() -1) //Output port
		mSenderList.at( port-mReceiverList.size() ).dataBuffer=data;
	else if ( port <= mReceiverList.size() + mSenderList.size() + mInControlList.size() -1) //Input control
		mInControlList.at( port-mReceiverList.size()-mSenderList.size() ).dataBuffer=data;
	else //Output control
		mOutControlList.at( port-mReceiverList.size()-mSenderList.size()-mInControlList.size() ).dataBuffer=data;
}



LADSPA_Descriptor * NetworkLADSPAPlugin::CreateLADSPADescriptor()
{
	CLAM::NetworkLADSPAPlugin plugin;

	unsigned numports =
		plugin.mReceiverList.size() + plugin.mSenderList.size() +
		plugin.mInControlList.size() + plugin.mOutControlList.size();

	if (numports == 0) return 0;

	LADSPA_Descriptor * descriptor = new LADSPA_Descriptor;

	descriptor->UniqueID = 8983;
	descriptor->Label = LadspaLibrary::dupstr("CLAMNetworkLADSPAPlugin");
	descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME;
	descriptor->Name = LadspaLibrary::dupstr("CLAM Network LADSPA Plugin");
	descriptor->Maker = LadspaLibrary::dupstr("CLAM-devel");
	descriptor->Copyright = LadspaLibrary::dupstr("GPL");
	descriptor->PortCount = numports;

	LADSPA_PortDescriptor * piPortDescriptors = new LADSPA_PortDescriptor[ numports ];
	descriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
	
	char ** pcPortNames = new char*[ numports ];
	
	descriptor->PortNames = (const char **)pcPortNames;

	LADSPA_PortRangeHint * psPortRangeHints = new LADSPA_PortRangeHint[ numports ];
	descriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;

	plugin.FillPortInfo( piPortDescriptors, pcPortNames, psPortRangeHints);
	
	descriptor->instantiate = ::Instantiate;
	descriptor->connect_port = ::ConnectTo;
	descriptor->activate = ::Activate;
	descriptor->run = ::Run;
	descriptor->deactivate = ::Deactivate;
	descriptor->cleanup = ::CleanUp;

	return descriptor;
}
} //end namespace CLAM

