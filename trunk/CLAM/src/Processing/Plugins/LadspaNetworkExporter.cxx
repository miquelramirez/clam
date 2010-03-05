#include "LadspaNetworkExporter.hxx"
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "AudioBufferSource.hxx"
#include "AudioBufferSink.hxx"
#include "NetworkPlayer.hxx"

namespace CLAM
{

class LadspaNetworkPlayer : public NetworkPlayer
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
	typedef std::vector< LADSPAInfo<ControlSource> > LADSPAInControlList;
	typedef std::vector< LADSPAInfo<ControlSink> > LADSPAOutControlList;
	typedef std::vector<LADSPA_Data * > Buffers;

	Network _network;
	Buffers _sourceBuffers;
	Buffers _sinkBuffers;
	Buffers _inControlBuffers;
	Buffers _outControlBuffers;
	LADSPAInControlList mInControlList;
	LADSPAOutControlList mOutControlList;
	unsigned long mClamBufferSize;
	unsigned long mExternBufferSize;
public:
	virtual bool IsWorking() { return true; }
	virtual std::string NonWorkingReason() { return ""; }
	virtual void Start() {}
	virtual void Stop() {}
//	virtual void Pause() {}
	virtual bool IsRealTime() const { return true; }
//	virtual unsigned BackendBufferSize();
//	virtual unsigned BackendSampleRate();
public:
	LadspaNetworkPlayer(const std::string & label, const std::string & networkXmlContent);
	~LadspaNetworkPlayer();

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
		return new CLAM::LadspaNetworkPlayer(descriptor->Label, (const char*)descriptor->ImplementationData);
	}
	// Destruct plugin instance
	static void CleanUp(LADSPA_Handle handle)
	{
//		std::cerr << "Network2Ladspa: cleanup " << handle << std::endl;
		delete (CLAM::LadspaNetworkPlayer*) handle;
	}

	// Run the plugin
	static void Run(LADSPA_Handle handle, unsigned long sampleCount)
	{
		CLAM::LadspaNetworkPlayer *p = (CLAM::LadspaNetworkPlayer*) handle;
		p->Run( sampleCount );
	}
	// Activate Plugin
	static void Activate(LADSPA_Handle handle)
	{
//		std::cerr << "Network2Ladspa: activate " << handle << std::endl;
		CLAM::LadspaNetworkPlayer *p = (CLAM::LadspaNetworkPlayer*) handle;
		p->Activate();
	}

	static void Deactivate(LADSPA_Handle handle)
	{
//		std::cerr << "Network2Ladspa: deactivate " << handle << std::endl;
		CLAM::LadspaNetworkPlayer *p = (CLAM::LadspaNetworkPlayer*) handle;
		p->Deactivate();
	}

	// Connect a port to a data location.
	static void ConnectTo(LADSPA_Handle handle, unsigned long port, LADSPA_Data * dataLocation)
	{
//		std::cerr << "Network2Ladspa: connect " << port << std::endl;
		CLAM::LadspaNetworkPlayer *p = (CLAM::LadspaNetworkPlayer*) handle;
		p->ConnectTo( port, dataLocation );
	}
}

namespace CLAM
{


LadspaNetworkPlayer::LadspaNetworkPlayer(const std::string & name, const std::string & networkXmlContent)
{
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

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

	// TOFIX: Should be a full link for the network to obtain parameters 
	// but the network adquires the ownership of the player and, in this case,
	// the player owns the network.
	SetNetworkBackLink(_network);
	LocateConnections();
}

LadspaNetworkPlayer::~LadspaNetworkPlayer()
{
}

void LadspaNetworkPlayer::Activate()
{
    
    CLAM_WARNING(_network.SupportsVariableAudioSize(), "Network don't support variable audio size");
	_network.Start();
}

void LadspaNetworkPlayer::Deactivate()
{
	_network.Stop();
}

void LadspaNetworkPlayer::LocateConnections()
{
	CacheSourcesAndSinks();

	CLAM_ASSERT( mInControlList.empty(), "LadspaNetworkPlayer::LocateConnections() : there are already registered controls");
	CLAM_ASSERT( mOutControlList.empty(), "LadspaNetworkPlayer::LocateConnections() : there are already registered controls");

	Network::ControlSources controlSources = _network.getOrderedControlSources();
	Network::ControlSinks controlSinks = _network.getOrderedControlSinks();

	_sourceBuffers.resize(GetNSources());
	_sinkBuffers.resize(GetNSinks());

	for (Network::ControlSources::const_iterator it=controlSources.begin(); it!=controlSources.end(); it++)
	{
		LADSPAInfo<ControlSource> info;
		info.name = _network.GetNetworkId(*it).c_str();
		info.processing=*it;
		mInControlList.push_back(info);
	}
	_inControlBuffers.resize(mInControlList.size());

	for (Network::ControlSinks::const_iterator it=controlSinks.begin(); it!=controlSinks.end(); it++)
	{
		LADSPAInfo<ControlSink> info;
		info.name = _network.GetNetworkId( *it ).c_str();
		info.processing =*it;
		mOutControlList.push_back(info);
	}
	_outControlBuffers.resize(mOutControlList.size());

	UpdatePortFrameAndHopSize();
}

void LadspaNetworkPlayer::UpdatePortFrameAndHopSize()
{
	//AudioSources and AudioBufferSource
	for (unsigned i=0; i<GetNSources(); i++)
	{
		SetSourceFrameSize(i, mExternBufferSize);
	}
	for (unsigned i=0; i<GetNSinks(); i++)
	{
		SetSinkFrameSize(i, mExternBufferSize);
	}
}

void LadspaNetworkPlayer::FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints )
{
	int currentport=0;

	//Manage InPorts (AudioSource and AudioBufferSources)
	for (unsigned i=0; i<GetNSources(); i++)
	{
		descriptors[currentport] = (LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO);
		names[currentport] = LadspaLibrary::dupstr( SourceName(i).c_str() );
		rangehints[currentport].HintDescriptor = 0;
		currentport++;
	}

	//Manage OutPorts (AudioSink and AudioSinksBuffer)
	for (unsigned i=0; i<GetNSinks(); i++)
	{
		descriptors[currentport] = (LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO);
		names[currentport] = LadspaLibrary::dupstr( SinkName(i).c_str() );
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

void LadspaNetworkPlayer::Run( unsigned long nsamples )
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

void LadspaNetworkPlayer::ProcessInControlValues()
{
	for (LADSPAInControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		it->processing->Do( (float) *(it->dataBuffer) );
}

void LadspaNetworkPlayer::ProcessOutControlValues()
{
	for (LADSPAOutControlList::iterator it=mOutControlList.begin(); it!=mOutControlList.end(); it++)
		*(it->dataBuffer)=it->processing->GetControlValue();
}

void LadspaNetworkPlayer::EstablishLadspaBuffersToAudioSources(const unsigned long nframes)
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceBuffer(i, _sourceBuffers[i], nframes);
}

void LadspaNetworkPlayer::EstablishLadspaBuffersToAudioSinks(const unsigned long nframes)
{
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkBuffer(i, _sinkBuffers[i], nframes);
}

void LadspaNetworkPlayer::ConnectTo(unsigned long port, LADSPA_Data * data)
{

	unsigned nSources  = GetNSources();
	unsigned nSinks  = GetNSinks();
	unsigned nInControls  = mInControlList.size();
	unsigned nOutControls  = mInControlList.size();

	if ( port < nSources ) //Input port
	{
		_sourceBuffers[port]=data;
		return;
	}
	port-=nSources;
	if (port < nSinks) //Output port
	{
		_sinkBuffers[port]=data;
		return;
	}
	port-=nSinks;
	if ( port < nInControls) //Input control
	{
		mInControlList.at( port ).dataBuffer=data;
		return;
	}
	port-=nInControls;
	if (port < nOutControls)
	{
		mOutControlList.at( port ).dataBuffer=data;
		return;
	}
}



LADSPA_Descriptor * LadspaNetworkPlayer::CreateLADSPADescriptor(
	const std::string & networkXmlContent,
	unsigned id,
	const std::string & label,
	const std::string & name,
	const std::string & maker,
	const std::string & copyright
	)
{
	CLAM::LadspaNetworkPlayer plugin(label, networkXmlContent);

	unsigned numports = plugin.GetNSources() + plugin.GetNSinks() +
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
	LADSPA_Descriptor * descriptor = LadspaNetworkPlayer::CreateLADSPADescriptor(
		networkXmlContent, id, label, name,
		maker, copyright);
	if (not descriptor) return;
	library.AddPluginType(descriptor);
}

} //end namespace CLAM

