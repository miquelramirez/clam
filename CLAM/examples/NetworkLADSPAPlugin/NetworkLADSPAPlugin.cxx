#include "NetworkLADSPAPlugin.hxx"

LADSPA_Descriptor * g_psDescriptor=NULL;
StartupShutdownHandler g_oShutdownStartupHandler;

////////////////////////////////////////////////////////////////////////////////////    HANDLER OBJECT
void StartupShutdownHandler::CreateLADSPADescriptor()
{
	std::cerr << " fill_ladspa_descriptor: " << std::endl;
	
	mPluginInstance=new CLAM::NetworkLADSPAPlugin();
	int numports=mPluginInstance->GetPortCount() + mPluginInstance->GetControlCount();

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psDescriptor = new LADSPA_Descriptor;

	//TODO posa cada grupet en mètodes propis: "registra portdescriptors...", etc
	if (g_psDescriptor)
	{
		g_psDescriptor->UniqueID = 8983;
		g_psDescriptor->Label = dupstr("CLAMNetworkLADSPAPlugin");
		g_psDescriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME;
		g_psDescriptor->Name = dupstr("CLAM Network LADSPA Plugin");
		g_psDescriptor->Maker = dupstr("CLAM-devel");
		g_psDescriptor->Copyright = dupstr("GPL");
		g_psDescriptor->PortCount = numports;

		std::cerr << " portcount="<<numports<<"" << std::endl;
		
		piPortDescriptors = new LADSPA_PortDescriptor[ numports ];
		g_psDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
		
		pcPortNames = new char*[ numports ];
		
		g_psDescriptor->PortNames = (const char **)pcPortNames;

		psPortRangeHints = new LADSPA_PortRangeHint[ numports ];
		g_psDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;
	
		mPluginInstance->FillPortInfo( piPortDescriptors, pcPortNames, psPortRangeHints);
		
		g_psDescriptor->instantiate = Instantiate;
		g_psDescriptor->connect_port = ConnectTo;
		g_psDescriptor->activate = Activate;
		g_psDescriptor->run = Run;
		g_psDescriptor->deactivate = Deactivate;
		g_psDescriptor->cleanup = CleanUp;
	}

}

StartupShutdownHandler::StartupShutdownHandler()
{
	std::cerr << " constructor handler" << std::endl;
	if (!g_psDescriptor)
		CreateLADSPADescriptor();
}

StartupShutdownHandler::~StartupShutdownHandler()
{
	delete mPluginInstance;
	
	std::cerr << " destructor handler" << std::endl;
	unsigned long lIndex;
	if (g_psDescriptor)
	{
		delete g_psDescriptor->Label;
		delete g_psDescriptor->Name;
		delete g_psDescriptor->Maker;
		delete g_psDescriptor->Copyright;
		delete g_psDescriptor->PortDescriptors;

		for (lIndex = 0; lIndex < g_psDescriptor->PortCount; lIndex++)
			delete g_psDescriptor->PortNames[lIndex];

		delete g_psDescriptor->PortNames;
		delete g_psDescriptor->PortRangeHints;
		delete g_psDescriptor;
	}
}

////////////////////////////////////////////////////////////////////////////////////    HANDLER OBJECT

// Return a descriptor of the requested plugin type.
const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	std::cerr << " ladspa_descriptor: " << index << std::endl;

	if (index == 0)
	{
		return g_psDescriptor;
	}
	else
		return NULL;
}


// Construct a new plugin instance.
LADSPA_Handle Instantiate(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate)
{
	std::cerr << " instantiate" << std::endl;

	//TODO això no està bé, principalment perquè si només té 1 in 1 out en vols intanciar dos però no li deixa...
	return g_oShutdownStartupHandler.mPluginInstance;
}

// Run the plugin
void Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;

	p->Run( SampleCount );
}

void CleanUp(LADSPA_Handle Instance)
{
	std::cerr << " cleanup " << std::endl;
}

void Activate(LADSPA_Handle Instance)
{
	std::cerr << " activate " << Instance << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;
	
	p->Activate();
	//Potser fer un START
}

void Deactivate(LADSPA_Handle Instance)
{
	std::cerr << " deactivate " << Instance << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;
	
	p->Deactivate();
	//Potser fer un STOP
}

/* Connect a port to a data location. */
void ConnectTo(LADSPA_Handle instance, unsigned long port, LADSPA_Data * dataLocation)
{
	((CLAM::NetworkLADSPAPlugin *)instance)->ConnectTo( port, dataLocation );
}
