
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
static CLAM::LadspaLibrary library;

CLAM_EXTERNAL_FILE_DATA(mono2bformat_embededNetwork,"../example-data/mono2bformat.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformatRotationZoom_embededNetwork,"../example-data/mono2bformat.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat2surround_embededNetwork,"../example-data/bformat2surround.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat2binaural_embededNetwork,"../example-data/bformat2binaural.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(mono2binaural_embededNetwork,"../example-data/mono2binaural.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(lalanetwork,"../lala.clamnetwork")

#include <iostream>

extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{

#if 0
CLAM::Network network;
CLAM::XMLStorage::Restore( network, "../example-data/bformat2binaural.clamnetwork");
return 0;
#endif

	static CLAM::LadspaNetworkExporter n100(library, mono2bformat_embededNetwork, 100,
			"mono2bformat", "Mono to BFormat",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n101(library, bformatRotationZoom_embededNetwork, 101,
			"bformat_rotation_zoom", "BFormat Rotation and Zoom",
			"BarcelonaMedia-Audio", "GNU GPL");
/*
	static CLAM::LadspaNetworkExporter n102(library, bformat2surround_embededNetwork, 102,
			"bformat2surround", "BFormat to Surround 5.0",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n103(library, bformat2binaural_embededNetwork, 103,
			"bformat2binaural", "BFormat to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n104(library, mono2binaural_embededNetwork, 104,
			"mono2binaural", "Mono to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n105(library, lalanetwork, 105,
			"dummy", "dummy network with configurations",
			"BarcelonaMedia-Audio", "GNU GPL");
*/

	return library.pluginAt(index);
}
