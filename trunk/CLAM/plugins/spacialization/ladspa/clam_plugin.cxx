
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>

CLAM_EXTERNAL_FILE_DATA(mono2bformat_embededNetwork,"../example-data/mono2bformat_nofaders.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformatRotationZoom_embededNetwork,"../example-data/bformatRotationZoom.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat2surround_embededNetwork,"../example-data/bformat2surround.clamnetwork")
//CLAM_EXTERNAL_FILE_DATA(bformat2binaural_embededNetwork,"../example-data/bformat2binaural.clamnetwork")
//CLAM_EXTERNAL_FILE_DATA(mono2binaural_embededNetwork,"../example-data/mono2binaural_nofaders.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat2cube_embededNetwork,"../example-data/bformat2cube.clamnetwork")

//fakes:
//CLAM_EXTERNAL_FILE_DATA(fake_bformat2surround_embededNetwork,"fake_bformat2surround.clamnetwork")
//CLAM_EXTERNAL_FILE_DATA(fake_bformat2binaural_embededNetwork,"fake_bformat2binaural.clamnetwork")

CLAM_EXTERNAL_FILE_DATA(vbap3d_embededNetwork,"../example-data/vbap3d_nofaders.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(vbap3d_dummy_22outs_embededNetwork,"../example-data/vbap3d_dummy_22outs_nofaders.clamnetwork")

CLAM_EXTERNAL_FILE_DATA(dummyControls_embededNetwork,"dummy_azimuth_elevation_controls.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat_to_15_embededNetwork,"../example-data/bformat2fifteen.clamnetwork")
CLAM_EXTERNAL_FILE_DATA(bformat_to_22_embededNetwork,"../example-data/bformat_to_22.clamnetwork")

#include <iostream>

extern "C" const LADSPA_Descriptor * ladspa_descriptor(unsigned long index)
{
	static CLAM::LadspaLibrary library;

	static CLAM::LadspaNetworkExporter n100(library, mono2bformat_embededNetwork, 100,
			"mono2bformat", "Mono to BFormat",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n101(library, bformatRotationZoom_embededNetwork, 101,
			"bformat_rotation_zoom", "BFormat Rotation and Zoom",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n102(library, bformat2surround_embededNetwork, 102,
			"bformat2surround", "BFormat to Surround 5.0",
			"BarcelonaMedia-Audio", "GNU GPL");

/*
	static CLAM::LadspaNetworkExporter n107(library, dummyControls_embededNetwork, 107,
			"dummy_controls", "Dummy controls for azimuth - elevation",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n103(library, bformat2binaural_embededNetwork, 103,
			"bformat2binaural", "BFormat to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n104(library, mono2binaural_embededNetwork, 104,
			"mono2binaural", "Mono to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n106(library, fake_bformat2surround_embededNetwork, 106,
			"bformat2surround", "BFormat to Surround 5.0",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n107(library, fake_bformat2binaural_embededNetwork, 107,
			"bformat2surround", "BFormat to binaural for headphones (using HRTF's)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n108(library, fake_bformat2binaural_embededNetwork, 108,
			"bformat2surround", "BFormat to stereo for speakers (located at -30 and 30 degrees)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n109(library, bformat2cube_embededNetwork, 109,
			"bformat2cube", "BFormat to Cube for 8 speakers",
			"BarcelonaMedia-Audio", "GNU GPL");
*/
	static CLAM::LadspaNetworkExporter n1015(library, vbap3d_embededNetwork, 1015,
			"id_vbap_15", "vbap15",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1022(library, vbap3d_dummy_22outs_embededNetwork, 1022,
			"id_vbap_22", "vbap22",
			"BarcelonaMedia-Audio", "GNU GPL");
	
	static CLAM::LadspaNetworkExporter n2015(library, bformat_to_15_embededNetwork, 2015,
			"id_bformat_15", "bformat15",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2022(library, bformat_to_22_embededNetwork, 2022,
			"id_bformat_22", "bformat22",
			"BarcelonaMedia-Audio", "GNU GPL");



	return library.pluginAt(index);
}
