#include "LV2NetworkPlayer.hxx"
#include "LV2Library.hxx"
#include <iostream>
// LV2 Callbacks
extern "C"
{
	static CLAM::LV2NetworkPlayer * exporter(LV2_Handle handle)
	{
		return (CLAM::LV2NetworkPlayer*)handle;
	}

	// Construct a new plugin instance.
	static LV2_Handle Instantiate(const LV2_Descriptor * descriptor, double sampleRate, const char *path, const LV2_Feature * const* features)
	{
	//	std::cout << "INSTANTIATE"<<std::endl;
		return new CLAM::LV2NetworkPlayer(descriptor, sampleRate, path, features);
	}

	// Destruct plugin instance
	static void CleanUp(LV2_Handle handle)
	{
	//	std::cout << "CLEANUP"<<std::endl;
		delete exporter(handle);
	}

	// Run the plugin
	static void Run(LV2_Handle handle,uint32_t sampleCount)
	{
	//	std::cout << "RUN" << std::endl;
		exporter(handle)->lv2_Run( sampleCount );
	}
	// Activate Plugin
	static void Activate(LV2_Handle handle)
	{
	//	std::cout << "ACTIVATE"<<std::endl;
		exporter(handle)->lv2_Activate();
	}

	static void Deactivate(LV2_Handle handle)
	{
	//	std::cout << "DEACTIVATE"<<std::endl;
		exporter(handle)->lv2_Deactivate();
	}

	// Connect a port to a data location.
	static void ConnectTo(LV2_Handle handle, uint32_t port,void* data)
	{
	//	std::cout << "CONNECT_2"<<std::endl;
		exporter(handle)->lv2_ConnectTo( port, data );
	}

}

namespace CLAM
{

//TODO Delete the networkXmlContent
LV2_Descriptor * LV2NetworkPlayer::CreateLV2Descriptor(
	const std::string & networkXmlContent,
	const std::string & uri
	)
{
	LV2_Descriptor * descriptor = new LV2_Descriptor;
	descriptor->URI            = LV2Library::dupstr(uri.c_str());
	descriptor->extension_data = NULL;
	descriptor->instantiate    = ::Instantiate;
	descriptor->connect_port   = ::ConnectTo;
	descriptor->activate       = ::Activate;
	descriptor->run            = ::Run;
	descriptor->deactivate     = ::Deactivate;
	descriptor->cleanup        = ::CleanUp;
	
	return descriptor;
}

LV2NetworkPlayer::LV2NetworkPlayer(const LV2_Descriptor * descriptor, double sampleRate, const char *path, const LV2_Feature * const* features)
	: mExternBufferSize(512)
{

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
	// TOFIX: Should be a full link for the network to obtain parameters 
	// but the network adquires the ownership of the player and, in this case,
	// the player owns the network.
	SetNetworkBackLink(_network);
	LocateConnections();
}

void LV2NetworkPlayer::LocateConnections()
{
	CacheSourcesAndSinks();

	_sourceBuffers.resize(GetNSources());
	_sinkBuffers.resize(GetNSinks());
	_inControlBuffers.resize(GetNControlSources());
	_outControlBuffers.resize(GetNControlSinks());

	ChangeFrameSize(mExternBufferSize);
}

LV2NetworkPlayer::~LV2NetworkPlayer()
{
}
void LV2NetworkPlayer::lv2_Activate()
{
	CLAM_WARNING(_network.SupportsVariableAudioSize(), "Network don't support variable audio size");
	_network.Start();
}
void LV2NetworkPlayer::lv2_Deactivate()
{
	_network.Stop();
}

void LV2NetworkPlayer::lv2_ConnectTo(uint32_t port, void *data)
{
	unsigned nSources  = GetNSources();
	unsigned nSinks  = GetNSinks();
	unsigned nInControls  = GetNControlSources();
	unsigned nOutControls  = GetNControlSinks();

	if ( port < nInControls) //Input control
	{
		_inControlBuffers[port]=(float*)data;
		return;
	}
	port-=nInControls;
	if (port < nOutControls)
	{
		_outControlBuffers[port]=(float*)data;
		return;
	}
	port-=nOutControls;
	if ( port < nSources ) //Input port
	{
		_sourceBuffers[port]=(float*)data;
		return;
	}
	port-=nSources;
	if (port < nSinks) //Output port
	{
		_sinkBuffers[port]=(float*)data;
		return;
	}
	CLAM_ASSERT(true,"Accessing a non-existing port");
}

void LV2NetworkPlayer::ChangeFrameSize(unsigned nframes)
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceFrameSize(i, nframes);
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkFrameSize(i, nframes);
}

void LV2NetworkPlayer::SetAudioSourceBuffers(unsigned long nframes)
{
	for (unsigned i=0; i<GetNSources(); i++)
		SetSourceBuffer(i,_sourceBuffers[i], nframes);
}

void LV2NetworkPlayer::SetAudioSinkBuffers(unsigned long nframes)
{
	for (unsigned i=0; i<GetNSinks(); i++)
		SetSinkBuffer(i,_sinkBuffers[i], nframes);
}

void LV2NetworkPlayer::ProcessInControlValues()
{
	for (unsigned i=0; i<GetNControlSources(); i++)
		ReadControlSource(i,_inControlBuffers[i]);
}

void LV2NetworkPlayer::ProcessOutControlValues()
{
	for (unsigned i=0; i<GetNControlSinks(); i++)
		FeedControlSink(i,_outControlBuffers[i]);
}


void LV2NetworkPlayer::lv2_Run(uint32_t nframes)
{

	if(nframes != mExternBufferSize)
	{
		mExternBufferSize=nframes;
		if(nframes == 0) return;
		ChangeFrameSize(mExternBufferSize);
	}

	ProcessInControlValues();
	SetAudioSourceBuffers(nframes);
	SetAudioSinkBuffers(nframes);

	_network.Do();
	ProcessOutControlValues();
}

}

