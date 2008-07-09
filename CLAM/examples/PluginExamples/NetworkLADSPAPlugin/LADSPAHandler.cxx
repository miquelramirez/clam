#include "LADSPAHandler.hxx"
#include "NetworkLADSPAPlugin.hxx"
#include <iostream>

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
	CLAM::NetworkLADSPAPlugin::DestructDescriptor(g_psDescriptor);
}

void StartupShutdownHandler::CreateLADSPADescriptor()
{
	g_psDescriptor = CLAM::NetworkLADSPAPlugin::CreateLADSPADescriptor();	
}

///      LADSPA CALLBACKS     ///////////////////////////////////////////////////

// Return a descriptor of the requested plugin type.
const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	std::cerr << "Network2Ladspa: ladspa_descriptor: " << index << std::endl;
	if (index == 0) return g_psDescriptor;
	return NULL;
}


