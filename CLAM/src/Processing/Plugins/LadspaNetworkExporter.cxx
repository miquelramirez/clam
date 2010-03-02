#include "LadspaNetworkExporter.hxx"
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "AudioSourceBuffer.hxx"
#include "AudioSinkBuffer.hxx"
#include "NetworkPlayer.hxx"

namespace CLAM
{

static std::string exportedName(const std::string & processingName, unsigned port, unsigned nports)
{
	std::stringstream portName;
	portName << processingName;
	if (nports > 1) portName << "_" << port;
	return portName.str();
}

class NetworkLADSPAPlugin
{
	template<class T>
	class LADSPAInfo
	{
	public:
		std::string name;
		T* processing;
		LADSPA_Data *dataBuffer;
		unsigned port;
	};

private:
	typedef std::vector< LADSPAInfo<Processing> > LADSPAInPortList;
	typedef std::vector< LADSPAInfo<Processing> > LADSPAOutPortList;

	typedef std::vector< LADSPAInfo<ControlSource> > LADSPAInControlList;
	typedef std::vector< LADSPAInfo<ControlSink> > LADSPAOutControlList;

	Network _network;
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;

	LADSPAInControlList mInControlList;
	LADSPAOutControlList mOutControlList;
	unsigned long mClamBufferSize, mExternBufferSize;

public:
	NetworkLADSPAPlugin(const std::string & label, const std::string & networkXmlContent);
	~NetworkLADSPAPlugin();

	void Activate();
	void Deactivate();

	void LocateConnections();
	void UpdatePortFrameAndHopSize();
	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);

	void Run( unsigned long nsamples );
	void EstablishLadspaBuffersToAudioSources(const unsigned long nframes);
	void EstablishLadspaBuffersToAudioSinks(const unsigned long nframes);
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

}

// Ladspa Callbacks
extern "C"
{
	// Construct a new plugin instance.
	static LADSPA_Handle Instantiate(const LADSPA_Descriptor * descriptor, unsigned long sampleRate)
	{
//		std::cerr << "Network2Ladspa: instantiate" << std::endl;
		return new CLAM::NetworkLADSPAPlugin(descriptor->Label, (const char*)descriptor->ImplementationData);
	}
	// Destruct plugin instance
	static void CleanUp(LADSPA_Handle handle)
	{
//		std::cerr << "Network2Ladspa: cleanup " << handle << std::endl;
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
//		std::cerr << "Network2Ladspa: activate " << handle << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->Activate();
	}

	static void Deactivate(LADSPA_Handle handle)
	{
//		std::cerr << "Network2Ladspa: deactivate " << handle << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->Deactivate();
	}

	// Connect a port to a data location.
	static void ConnectTo(LADSPA_Handle handle, unsigned long port, LADSPA_Data * dataLocation)
	{
//		std::cerr << "Network2Ladspa: connect " << port << std::endl;
		CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
		p->ConnectTo( port, dataLocation );
	}
}

namespace CLAM
{


NetworkLADSPAPlugin::NetworkLADSPAPlugin(const std::string & name, const std::string & networkXmlContent)
{
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

//	std::cerr << "NetworkLADSPAPlugin: Constructed" << std::endl;
	std::istringstream xmlfile(networkXmlContent);
	try
	{
		XmlStorage::Restore( _network, xmlfile );
	}
	catch ( XmlStorageErr err)
	{
		std::cerr << "CLAM LADSPA: Error while loading CLAM network based plugin '" << name << "'." <<std::endl;
		std::cerr << err.what() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}
	if (_network.HasMisconfiguredProcessings())
	{
		std::cerr << "CLAM LADSPA: Error while configuring CLAM network based plugin '" << name << "'." <<std::endl;
		std::cerr << _network.GetConfigurationErrors() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}
	if (_network.HasUnconnectedInPorts())
	{
		std::cerr << "CLAM LADSPA: Error loading CLAM network based plugin '" << name << "'." <<std::endl;
		std::cerr << "Plugin not loaded because internal network inports were unconnected." <<std::endl;
		std::cerr << _network.GetUnconnectedInPorts() << std::flush;
		return;
	}

	LocateConnections();
}

NetworkLADSPAPlugin::~NetworkLADSPAPlugin()
{
//	std::cerr << "NetworkLADSPAPlugin: DELETED" << std::endl;
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

	Network::Processings sources=_network.getOrderedProcessingsByAttribute("port_source_type");
	Network::Processings sinks=_network.getOrderedProcessingsByAttribute("port_sink_type");

	Network::ControlSources controlSources = _network.getOrderedControlSources();
	Network::ControlSinks controlSinks = _network.getOrderedControlSinks();

	for (Network::Processings::const_iterator it=sources.begin(); it!=sources.end(); it++)
	{
		std::string processingName = _network.GetNetworkId(*it);

		Processing * source =(*it);
		unsigned nports = source->GetNOutPorts();
		for(unsigned port = 0; port < nports; ++port)
		{
			std::string portName = exportedName(processingName,port,nports);
			LADSPAInfo<Processing> info;
			info.name = portName.c_str();
			info.port = port;
			info.processing=source;
			mReceiverList.push_back(info);
		}
	}

	for (Network::Processings::const_iterator it=sinks.begin(); it!=sinks.end(); it++)
	{
		std::string processingName = _network.GetNetworkId( *it );

		Processing * sink =(*it);
		unsigned nports = sink->GetNInPorts();
		for(unsigned port = 0; port < nports; ++port)
		{
			std::string portName = exportedName(processingName,port,nports);
			LADSPAInfo<Processing> info;
			info.name = portName.c_str();
			info.port = port;
			info.processing = sink;
			mSenderList.push_back(info);
		}
	}

	for (Network::ControlSources::const_iterator it=controlSources.begin(); it!=controlSources.end(); it++)
	{
		LADSPAInfo<ControlSource> info;
		info.name = _network.GetNetworkId(*it).c_str();
		info.processing=*it;
		mInControlList.push_back(info);
	}

	for (Network::ControlSinks::const_iterator it=controlSinks.begin(); it!=controlSinks.end(); it++)
	{
		LADSPAInfo<ControlSink> info;
		info.name = _network.GetNetworkId( *it ).c_str();
		info.processing =*it;
		mOutControlList.push_back(info);
	}
	UpdatePortFrameAndHopSize();
}

void NetworkLADSPAPlugin::UpdatePortFrameAndHopSize()
{
	//AudioSources and AudioSourceBuffer
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		Processing * processing = it->processing;
		unsigned nports = processing->GetNOutPorts();
		for(unsigned port = 0; port < nports; ++port)
		{
			if(typeid(*processing)==typeid(AudioSource))
				((AudioSource*)processing)->SetFrameAndHopSize( mExternBufferSize, port );
			else // AuidoSourceBuffer
				((AudioSourceBuffer*)processing)->SetFrameAndHopSize( 1, port );
		}
	}
	//AudioSinks and AudioSinksBuffer
	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		Processing * processing = it->processing;
		unsigned nports = processing->GetNInPorts();
		for(unsigned port = 0; port < nports; ++port)
		{
			if(typeid(*processing)==typeid(AudioSink))
				((AudioSink*)processing)->SetFrameAndHopSize ( mExternBufferSize, port );
			else // AuidoSourceBuffer
				((AudioSinkBuffer*)processing)->SetFrameAndHopSize ( 1, port );
		}
	}
}

void NetworkLADSPAPlugin::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
{
	int currentport=0;

	//Manage InPorts (AudioSource and AudioSourcesBuffer)
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
		names[currentport] = LadspaLibrary::dupstr( it->name.c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage OutPorts (AudioSink and AudioSinksBuffer)
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

		//Obté processingConfig, i defineix cada param
		ControlSourceConfig conf = dynamic_cast<const ControlSourceConfig&>(it->processing->GetConfig() );

		rangehints[currentport].LowerBound=(LADSPA_Data)conf.GetMinValue();
		rangehints[currentport].UpperBound=(LADSPA_Data)conf.GetMaxValue();

		unsigned defaultHintValue=LADSPA_HINT_DEFAULT_MIDDLE;
		// specific cases when the default value is equal to some of defined LADSPA hints (ugly!):
		if (conf.GetDefaultValue() == conf.GetMinValue() ) defaultHintValue=LADSPA_HINT_DEFAULT_MINIMUM;
		else if (conf.GetDefaultValue() == conf.GetMaxValue() ) defaultHintValue=LADSPA_HINT_DEFAULT_MAXIMUM;
		else if (conf.GetDefaultValue() == 0) defaultHintValue = LADSPA_HINT_DEFAULT_0;
		else if (conf.GetDefaultValue() == 1) defaultHintValue = LADSPA_HINT_DEFAULT_1;
		else if (conf.GetDefaultValue() == 100) defaultHintValue = LADSPA_HINT_DEFAULT_100;
		else if (conf.GetDefaultValue() == 440) defaultHintValue = LADSPA_HINT_DEFAULT_440;

		rangehints[currentport].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | defaultHintValue);
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
		if (nsamples==0)
			return; // Seems that in Ardour2.8 it does never runs plugins with 0 samples
		UpdatePortFrameAndHopSize();
	}

	ProcessInControlValues();

	EstablishLadspaBuffersToAudioSources(nsamples);
	EstablishLadspaBuffersToAudioSinks(nsamples);
	//Do() as much as it is needed
//	for (int stepcount=0; stepcount < (int(mExternBufferSize)/int(mClamBufferSize)); stepcount++)
	{
		_network.Do();
//		if (stepcount>0) std::cout << "ieeps!" << std::flush;
	}

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

void NetworkLADSPAPlugin::EstablishLadspaBuffersToAudioSources(const unsigned long nframes)
{
	for (LADSPAInPortList::iterator it=mReceiverList.begin(); it!=mReceiverList.end(); it++)
	{
		Processing * source = it->processing;
		if(typeid(*source)==typeid(AudioSource))
			((AudioSource*)source)->SetExternalBuffer(it->dataBuffer, nframes, it->port );
		else // AuidoSourceBuffer
			((AudioSourceBuffer*)source)->SetExternalBuffer(it->dataBuffer, nframes, it->port );
	}
}

void NetworkLADSPAPlugin::EstablishLadspaBuffersToAudioSinks(const unsigned long nframes)
{
   	for (LADSPAOutPortList::iterator it=mSenderList.begin(); it!=mSenderList.end(); it++)
	{
		Processing * sink = it->processing;
		if(typeid(*sink )==typeid(AudioSink))
			((AudioSink*)sink)->SetExternalBuffer(it->dataBuffer, nframes, it->port );
		else // AuidoSourceBuffer
			((AudioSinkBuffer*)sink)->SetExternalBuffer(it->dataBuffer, nframes, it->port );
	}
}

void NetworkLADSPAPlugin::ConnectTo(unsigned long port, LADSPA_Data * data)
{

	unsigned sizeRecList  = mReceiverList.size();
	unsigned sizeSenList  = mSenderList.size();
	unsigned sizeInList  = mInControlList.size();

	if ( port < sizeRecList ) //Input port
	{
		mReceiverList.at( port ).dataBuffer=data;
	}
	else if ( port < sizeRecList + sizeSenList) //Output port
	{
		mSenderList.at( port - sizeRecList ).dataBuffer=data;
	}
	else if ( port < sizeRecList + sizeSenList + sizeInList) //Input control
	{
		mInControlList.at( port - sizeRecList - sizeSenList ).dataBuffer=data;
	}
	else
	{	//Output control
		mOutControlList.at( port - sizeRecList - sizeSenList - sizeInList ).dataBuffer=data;
	}
}



LADSPA_Descriptor * NetworkLADSPAPlugin::CreateLADSPADescriptor(
	const std::string & networkXmlContent,
	unsigned id,
	const std::string & label,
	const std::string & name,
	const std::string & maker,
	const std::string & copyright
	)
{
	CLAM::NetworkLADSPAPlugin plugin(label, networkXmlContent);

	unsigned numports =
		plugin.mReceiverList.size() + plugin.mSenderList.size() +
		plugin.mInControlList.size() + plugin.mOutControlList.size();

	if (numports == 0) return 0;

	LADSPA_Descriptor * descriptor = new LADSPA_Descriptor;

	descriptor->UniqueID = id;
	descriptor->Label = LadspaLibrary::dupstr(label.c_str());
	descriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME;
	descriptor->Name = LadspaLibrary::dupstr(name.c_str());
	descriptor->Maker = LadspaLibrary::dupstr(maker.c_str());
	descriptor->Copyright = LadspaLibrary::dupstr(copyright.c_str());
	descriptor->ImplementationData = LadspaLibrary::dupstr(networkXmlContent.c_str());
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

LadspaNetworkExporter::LadspaNetworkExporter(
	LadspaLibrary & library, const std::string & networkXmlContent,
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

} //end namespace CLAM

