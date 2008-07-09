#include "LADSPAHandler.hxx"
#include "NetworkLADSPAPlugin.hxx"
#include <iostream>

extern "C"
{
	const LADSPA_Descriptor * ladspa_descriptor(unsigned long);
	LADSPA_Handle Instantiate(const LADSPA_Descriptor *, unsigned long);
	void Run(LADSPA_Handle, unsigned long);
	void CleanUp(LADSPA_Handle);
	void Activate(LADSPA_Handle);
	void Deactivate(LADSPA_Handle);
	void ConnectTo(LADSPA_Handle, unsigned long, LADSPA_Data *);
}

static LADSPA_Descriptor * g_psDescriptor=0;
static StartupShutdownHandler g_oShutdownStartupHandler;

///    HANDLER OBJECT     /////////////////////////////////////////////////
StartupShutdownHandler::StartupShutdownHandler()
{
	std::cerr << "Network2Ladspa: constructor handler" << std::endl;
	if (!g_psDescriptor)
		CreateLADSPADescriptor();
}

StartupShutdownHandler::~StartupShutdownHandler()
{
	std::cerr << "Network2Ladspa: destructor handler" << std::endl;
	if (g_psDescriptor)
	{
		delete g_psDescriptor->Label;
		delete g_psDescriptor->Name;
		delete g_psDescriptor->Maker;
		delete g_psDescriptor->Copyright;
		delete g_psDescriptor->PortDescriptors;

		for (unsigned long lIndex = 0; lIndex < g_psDescriptor->PortCount; lIndex++)
			delete g_psDescriptor->PortNames[lIndex];

		delete g_psDescriptor->PortNames;
		delete g_psDescriptor->PortRangeHints;
		delete g_psDescriptor;
	}
}

void StartupShutdownHandler::CreateLADSPADescriptor()
{	
	CLAM::NetworkLADSPAPlugin *plugin=new CLAM::NetworkLADSPAPlugin();
	int numports=plugin->GetPortCount() + plugin->GetControlCount();

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psDescriptor = new LADSPA_Descriptor;

	if (g_psDescriptor)
	{
		g_psDescriptor->UniqueID = 8983;
		g_psDescriptor->Label = dupstr("CLAMNetworkLADSPAPlugin");
		g_psDescriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME;
		g_psDescriptor->Name = dupstr("CLAM Network LADSPA Plugin");
		g_psDescriptor->Maker = dupstr("CLAM-devel");
		g_psDescriptor->Copyright = dupstr("GPL");
		g_psDescriptor->PortCount = numports;

		piPortDescriptors = new LADSPA_PortDescriptor[ numports ];
		g_psDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
		
		pcPortNames = new char*[ numports ];
		
		g_psDescriptor->PortNames = (const char **)pcPortNames;

		psPortRangeHints = new LADSPA_PortRangeHint[ numports ];
		g_psDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;
	
		plugin->FillPortInfo( piPortDescriptors, pcPortNames, psPortRangeHints);
		
		g_psDescriptor->instantiate = Instantiate;
		g_psDescriptor->connect_port = ConnectTo;
		g_psDescriptor->activate = Activate;
		g_psDescriptor->run = Run;
		g_psDescriptor->deactivate = Deactivate;
		g_psDescriptor->cleanup = CleanUp;
	}

	delete plugin;
}

///      LADSPA CALLBACKS     ///////////////////////////////////////////////////

// Return a descriptor of the requested plugin type.
const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	std::cerr << "Network2Ladspa: ladspa_descriptor: " << index << std::endl;
	if (index == 0) return g_psDescriptor;
	return NULL;
}


// Construct a new plugin instance.
LADSPA_Handle Instantiate(const LADSPA_Descriptor * descriptor, unsigned long sampleRate)
{
	std::cerr << "Network2Ladspa: instantiate" << std::endl;
	return new CLAM::NetworkLADSPAPlugin();
}

void CleanUp(LADSPA_Handle handle)
{
	std::cerr << "Network2Ladspa: cleanup " << handle << std::endl;
	delete (CLAM::NetworkLADSPAPlugin*) handle;
}

// Run the plugin
void Run(LADSPA_Handle handle, unsigned long sampleCount)
{
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
	p->Run( sampleCount );
}

void Activate(LADSPA_Handle handle)
{
	std::cerr << "Network2Ladspa: activate " << handle << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
	p->Activate();
}

void Deactivate(LADSPA_Handle handle)
{
	std::cerr << "Network2Ladspa: deactivate " << handle << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
	p->Deactivate();
}

// Connect a port to a data location.
void ConnectTo(LADSPA_Handle handle, unsigned long port, LADSPA_Data * dataLocation)
{
	std::cerr << "Network2Ladspa: connect " << port << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) handle;
	p->ConnectTo( port, dataLocation );
}

