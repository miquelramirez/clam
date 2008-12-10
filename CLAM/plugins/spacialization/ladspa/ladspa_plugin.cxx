#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
static CLAM::LadspaLibrary library;

CLAM_EXTERNAL_FILE_DATA(mono2bformat_embededNetwork,"../example-data/mono2bformat.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformatRotationZoom_embededNetwork,"../example-data/mono2bformat.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat2binaural_embededNetwork,"../example-data/bformat2binaural.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(mono2binaural_embededNetwork,"../example-data/mono2binaural.clamnetwork")

extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{

	static CLAM::LadspaNetworkExporter n100(library, mono2bformat_embededNetwork, 100,
			"mono2bformat", "Mono to BFormat",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n101(library, bformatRotationZoom_embededNetwork, 101,
			"bformat_rotation_zoom", "BFormat Rotation and Zoom",
			"BarcelonaMedia-Audio", "GNU GPL");
/*
	static CLAM::LadspaNetworkExporter n102(library, bformat2binaural_embededNetwork, 102,
			"bformat2binaural", "BFormat to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n103(library, mono2binaural_embededNetwork, 103,
			"mono2binaural", "Mono to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");
*/
	return library.pluginAt(index);
}
