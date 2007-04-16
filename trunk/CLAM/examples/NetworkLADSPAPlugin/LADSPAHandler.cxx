#include "LADSPAHandler.hxx"
#include "NetworkLADSPAPlugin.hxx"
#include <iostream>

LADSPA_Descriptor * g_psDescriptor=NULL;
StartupShutdownHandler g_oShutdownStartupHandler;

////////////////////////////////////////////////////////////////////////////////////    HANDLER OBJECT
StartupShutdownHandler::StartupShutdownHandler()
{
	std::cerr << " constructor handler" << std::endl;
	if (!g_psDescriptor)
		CreateLADSPADescriptor();
}

StartupShutdownHandler::~StartupShutdownHandler()
{
	std::cerr << " destructor handler" << std::endl;
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

////////////////////////////////////////////////////////////////////////////////////    LADSPA CALLBACKS

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
	return new CLAM::NetworkLADSPAPlugin();
}

// Run the plugin
void Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;

	p->Run( SampleCount );
}

void CleanUp(LADSPA_Handle Instance)
{
	std::cerr << " cleanup " << Instance << std::endl;
	//Cleanup method only called by some apps. Jack-rack doesn't, but Sweep does.
	delete (CLAM::NetworkLADSPAPlugin*) Instance;
}

void Activate(LADSPA_Handle Instance)
{
	std::cerr << " activate " << Instance << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;

	//Start network
	p->Activate();
}

void Deactivate(LADSPA_Handle Instance)
{
	std::cerr << " deactivate " << Instance << std::endl;
	CLAM::NetworkLADSPAPlugin *p = (CLAM::NetworkLADSPAPlugin*) Instance;
	
	//Stop network
	p->Deactivate();
}

/* Connect a port to a data location. */
void ConnectTo(LADSPA_Handle instance, unsigned long port, LADSPA_Data * dataLocation)
{
	((CLAM::NetworkLADSPAPlugin *)instance)->ConnectTo( port, dataLocation );
}

