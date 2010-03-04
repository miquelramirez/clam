#include "Exporter.hxx"
#include "LV2Library.hxx"

// LV2 Callbacks
extern "C"
{
	static CLAM::Exporter * exporter(LV2_Handle handle)
	{
		return (CLAM::Exporter*)handle;
	}

	// Construct a new plugin instance.
	static LV2_Handle Instantiate(const LV2_Descriptor * descriptor, double sampleRate, const char *path, const LV2_Feature * const* features)
	{
//		std::cout << "INSTANTIATE"<<std::endl;
		return new CLAM::Exporter(descriptor);
	}

	// Destruct plugin instance
	static void CleanUp(LV2_Handle handle)
	{
//		std::cout << "CLEANUP"<<std::endl;
		delete exporter(handle);
	}

	// Run the plugin
	static void Run(LV2_Handle handle,uint32_t sampleCount)
	{
//		std::cout << "RUN" << std::endl;
		exporter(handle)->RunExporter( sampleCount );
	}
	// Activate Plugin
	static void Activate(LV2_Handle handle)
	{
//		std::cout << "ACTIVATE"<<std::endl;
		exporter(handle)->ActivateExporter();
	}
	
	static void Deactivate(LV2_Handle handle)
	{
//		std::cout << "DEACTIVATE"<<std::endl;
		exporter(handle)->DeactivateExporter();
	}

	// Connect a port to a data location.
	static void ConnectTo(LV2_Handle handle, uint32_t port,void* data)
	{
//		std::cout << "CONNECT_2"<<std::endl;
		exporter(handle)->ConnectPortExporter( port, data );
	}

/*	This function is necessary when creates the plugin without library
	LV2_SYMBOL_EXPORT
	const LV2_Descriptor *lv2_descriptor( uint32_t index)
	{
		std::cout << "CREATE_LV2_DESCRIPTOR"<<std::endl;
		
		if(index==0) {
			return Exporter::CreateLV2Descriptor();		
		}
		else
			return NULL;
	}
*/
}

namespace CLAM
{

Exporter::Exporter(const LV2_Descriptor * descriptor)
{
	mClamBufferSize=512;
	mExternBufferSize=mClamBufferSize;

	std::istringstream xmlfile(LV2Library::getNetwork(descriptor->URI));

	try
	{
		CLAM::XmlStorage::Restore( _network, xmlfile);
	}
	catch ( CLAM::XmlStorageErr err)
	{
		std::cerr << "CLAM LV2: Error while loading CLAM network based plugin '" << descriptor->URI<< "'." <<std::endl;
		std::cerr << err.what() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}

	if (_network.HasMisconfiguredProcessings())
	{
		std::cerr << "CLAM LV2: Error while configuring CLAM network based plugin '"<< descriptor->URI<< "'." <<std::endl;
		std::cerr << _network.GetConfigurationErrors() << std::endl;
		std::cerr << "Plugin not loaded." << std::endl;
		return;
	}

	if (_network.HasUnconnectedInPorts())
	{
		std::cerr << "CLAM LV2: Error loading CLAM network based plugin '" <<descriptor->URI<< "'." <<std::endl;
		std::cerr << "Plugin not loaded because internal network inports were unconnected." <<std::endl;
		std::cerr << _network.GetUnconnectedInPorts() << std::flush;
		return;
	}

	LocateConnections();
}

void Exporter::LocateConnections()
{

//	CHECK THAT ALL VECTORS AREN'T EMPTY
//	CLAM_ASSERT( mReceiverList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered input ports");
//	CLAM_ASSERT( mSenderList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered output ports");
//	CLAM_ASSERT( mInControlList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered controls");
//	CLAM_ASSERT( mOutControlList.empty(), "NetworkLADSPAPlugin::LocateConnections() : there are already registered controls");
	CacheSourcesAndSinks();

	_sourceBuffers.resize(GetNSources());
	for (unsigned i=0; i<GetNSources(); i++)
	{
		SetSourceFrameSize(i, mExternBufferSize);
	}
	_sinkBuffers.resize(GetNSinks());
	for (unsigned i=0; i<GetNSinks(); i++)
	{
		SetSinkFrameSize(i, mExternBufferSize);
	}

	InControlList controlSources = _network.getOrderedControlSources();
	for (CLAM::Network::ControlSources::const_iterator it=controlSources.begin(); it!=controlSources.end(); it++)
	{	//std::cout<<"INSERT CONTROL SOURCE"<<std::endl;
		LV2Info<CLAM::ControlSource> info;
		info.name = _network.GetNetworkId(*it).c_str();
		info.processing=*it;
		mInControlList.push_back(info);
	}
	_inControlBuffers.resize(mInControlList.size());

	OutControlList controlSinks  = _network.getOrderedControlSinks();
	for (CLAM::Network::ControlSinks::const_iterator it=controlSinks.begin(); it!=controlSinks.end(); it++)
	{	//std::cout<<"INSERT CONTROL SINK"<<std::endl;
		LV2Info<CLAM::ControlSink> info;
		info.name = _network.GetNetworkId( *it ).c_str();
		info.processing =*it;
		mOutControlList.push_back(info);	
	}
	_outControlBuffers.resize(mOutControlList.size());
}

Exporter::~Exporter(){}
void Exporter::ActivateExporter()
{
	_network.Start();
}
void Exporter::DeactivateExporter()
{
	_network.Stop();
}
void Exporter::CleanupExporter(){}
void Exporter::InstantiateExporter(){}

void Exporter::ConnectPortExporter(uint32_t port, void *data)
{
	if ( port < _inControlBuffers.size() )
	{
		_inControlBuffers[port]=(float*)data;
		return;
	}
	port-=_inControlBuffers.size();
	if ( port < _outControlBuffers.size())
	{
		_outControlBuffers[port]=(float*)data;
		return;
	}
	port-=_outControlBuffers.size();
	if ( port < _sourceBuffers.size() )
	{
		_sourceBuffers[port]=(float*)data;
		return;
	}
	port-=_sourceBuffers.size();
	if ( port < _sinkBuffers.size() )
	{
		_sinkBuffers[port]=(float*)data;
		return;
	}
	CLAM_ASSERT(true,"Accessing a non-existing port");
}
	
void Exporter::UpdatePortFrameAndHopSize()
{	
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceFrameSize(i, mExternBufferSize);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkFrameSize(i, mExternBufferSize);
}

void Exporter::SetAudioSourceBuffers(const unsigned long nframes)
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceBuffer(i,_sourceBuffers[i], nframes);
}

void Exporter::SetAudioSinkBuffers(const unsigned long nframes)
{
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkBuffer(i,_sinkBuffers[i], nframes);
}

void Exporter::ProcessInControlValues()
{
	for (LV2InControlList::iterator it=mInControlList.begin(); it!=mInControlList.end(); it++)
		it->processing->Do( (float) *(it->dataBuffer) );
}

void Exporter::ProcessOutControlValues()
{

}	


void Exporter::RunExporter(uint32_t nframes)
{
	if(nframes != mExternBufferSize)
	{
		mExternBufferSize=nframes;
		if(nframes == 0) return;		
		UpdatePortFrameAndHopSize();
	}
	ProcessInControlValues();
	SetAudioSourceBuffers(nframes);
	SetAudioSinkBuffers(nframes);  		
	_network.Do();
	ProcessOutControlValues();
}

//TODO Delete the networkXmlContent
LV2_Descriptor * Exporter::CreateLV2Descriptor(
	const std::string & networkXmlContent,
	const std::string & uri
	)
{
	// HERE WRITE THE RDF	

	LV2_Descriptor * descriptor = new LV2_Descriptor;
	
	descriptor->URI 		= LV2Library::dupstr(uri.c_str());
	descriptor->extension_data	= NULL;
	descriptor->instantiate = ::Instantiate;
	descriptor->connect_port = ::ConnectTo;
	descriptor->activate = ::Activate;
	descriptor->run = ::Run;
	descriptor->deactivate = Deactivate;
	descriptor->cleanup = ::CleanUp;

	return descriptor;
}

}
