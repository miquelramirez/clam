
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
#include <CLAM/EmbeddedFile.hxx>


CLAM_EMBEDDED_FILE(osc_embededNetwork,"osc_sender.clamnetwork")

#include <iostream>
extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	static CLAM::LadspaLibrary library;

	static CLAM::LadspaNetworkExporter n10000(library, osc_embededNetwork, 10000,
			"CLAM_OSC_control_sender", "CLAM OSC Control Sender",
			"BarcelonaMedia-Audio", "GNU GPL");
	return library.pluginAt(index);
}
