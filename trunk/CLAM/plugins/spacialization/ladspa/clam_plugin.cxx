
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
#include <CLAM/EmbeddedFile.hxx>


CLAM_EMBEDDED_FILE(bformatrotation_osc_embededNetwork,"bformatrotation.clamnetwork")

CLAM_EMBEDDED_FILE(mono2binaural_osc_embededNetwork,"../example-data/mono2binaural_osc.clamnetwork")
CLAM_EMBEDDED_FILE(bformat2binaural_rotation_osc_embededNetwork,"../example-data/bformat2binaural_rotation_osc.clamnetwork")

CLAM_EMBEDDED_FILE(mono2bformat_embededNetwork,"../example-data/mono2bformat_nofaders.clamnetwork")
CLAM_EMBEDDED_FILE(bformatRotationZoom_embededNetwork,"../example-data/bformatRotationZoom.clamnetwork")
CLAM_EMBEDDED_FILE(bformat2surround_embededNetwork,"../example-data/bformat2surround.clamnetwork")
CLAM_EMBEDDED_FILE(bformat2binaural_embededNetwork,"../example-data/bformat2binaural.clamnetwork")
CLAM_EMBEDDED_FILE(mono2binaural_embededNetwork,"../example-data/mono2binaural_ladspa.clamnetwork")
CLAM_EMBEDDED_FILE(bformat2cube_embededNetwork,"../example-data/bformat2cube.clamnetwork")

//fakes:
//CLAM_EMBEDDED_FILE(fake_bformat2surround_embededNetwork,"fake_bformat2surround.clamnetwork")
//CLAM_EMBEDDED_FILE(fake_bformat2binaural_embededNetwork,"fake_bformat2binaural.clamnetwork")

CLAM_EMBEDDED_FILE(vbap3d_embededNetwork,"../example-data/vbap3d_nofaders.clamnetwork")
CLAM_EMBEDDED_FILE(vbap3d_dummy_22outs_embededNetwork,"../example-data/vbap3d_dummy_22outs_nofaders.clamnetwork")

CLAM_EMBEDDED_FILE(dummyControls_embededNetwork,"dummy_azimuth_elevation_controls.clamnetwork")
CLAM_EMBEDDED_FILE(bformat_to_15_embededNetwork,"../example-data/bformat2fifteen.clamnetwork")
CLAM_EMBEDDED_FILE(bformat_to_22_embededNetwork,"../example-data/bformat_to_22.clamnetwork")

CLAM_EMBEDDED_FILE(bformat06,"bformat06.clamnetwork")
CLAM_EMBEDDED_FILE(bformat14,"bformat14.clamnetwork")
CLAM_EMBEDDED_FILE(bformat15,"bformat15.clamnetwork")
CLAM_EMBEDDED_FILE(bformat16,"bformat16.clamnetwork")
CLAM_EMBEDDED_FILE(bformat17,"bformat17.clamnetwork")
CLAM_EMBEDDED_FILE(bformat18,"bformat18.clamnetwork")
CLAM_EMBEDDED_FILE(bformat19,"bformat19.clamnetwork")
CLAM_EMBEDDED_FILE(bformat20,"bformat20.clamnetwork")
CLAM_EMBEDDED_FILE(bformat21,"bformat21.clamnetwork")
CLAM_EMBEDDED_FILE(bformat22,"bformat22.clamnetwork")

CLAM_EMBEDDED_FILE(vbap14,"vbap14.clamnetwork")
CLAM_EMBEDDED_FILE(vbap15,"vbap15.clamnetwork")
CLAM_EMBEDDED_FILE(vbap15_shorter_distance,"vbap15_shorter_distance.clamnetwork")
CLAM_EMBEDDED_FILE(vbap16,"vbap16.clamnetwork")
CLAM_EMBEDDED_FILE(vbap17,"vbap17.clamnetwork")
CLAM_EMBEDDED_FILE(vbap18,"vbap18.clamnetwork")
CLAM_EMBEDDED_FILE(vbap19,"vbap19.clamnetwork")
CLAM_EMBEDDED_FILE(vbap20,"vbap20.clamnetwork")
CLAM_EMBEDDED_FILE(vbap21,"vbap21.clamnetwork")
CLAM_EMBEDDED_FILE(vbap22,"vbap22.clamnetwork")

CLAM_EMBEDDED_FILE(decoded_bformat15,"decoded_bformat15.clamnetwork")

#include <iostream>
/*
Numbering:
100-110 misc
10XX vbap XX channels
20XX bformat XX channels
30XX hoa? testing
*/
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
	static CLAM::LadspaNetworkExporter n103(library, bformat2binaural_embededNetwork, 103,
			"bformat2binaural", "BFormat to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n104(library, mono2binaural_embededNetwork, 104,
			"mono2binaural", "Mono to binaural (HRTF)",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n110(library, bformatrotation_osc_embededNetwork, 110,
			"bformat_rotation_zoom", "BFormat rotation and zoom (osc controls)",
			"BarcelonaMedia-Audio", "GNU GPL");

/*
	static CLAM::LadspaNetworkExporter n203(library, bformat2binaural_rotation_osc_embededNetwork, 203,
			"bformat2binaural_rotation_osc", "BFormat to binaural (HRTF) with headtracking",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n204(library, mono2binaural_osc_embededNetwork, 204,
			"mono2binaural_osc", "Mono to Binaural (HRTF) with headtracking",
			"BarcelonaMedia-Audio", "GNU GPL");
*/
/*
	static CLAM::LadspaNetworkExporter n107(library, dummyControls_embededNetwork, 107,
			"dummy_controls", "Dummy controls for azimuth - elevation",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n106(library, fake_bformat2surround_embededNetwork, 108,
			"bformat2surround", "BFormat to Surround 5.0",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n107(library, fake_bformat2binaural_embededNetwork, 109,
			"bformat2surround", "BFormat to binaural for headphones (using HRTF's)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n108(library, fake_bformat2binaural_embededNetwork, 110,
			"bformat2surround", "BFormat to stereo for speakers (located at -30 and 30 degrees)",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n111(library, bformat2cube_embededNetwork, 111,
			"bformat2cube", "BFormat to Cube for 8 speakers",
			"BarcelonaMedia-Audio", "GNU GPL");
*/

	static CLAM::LadspaNetworkExporter n1115(library, vbap15_shorter_distance, 1115,
			"id_vbap_15_shorter_distance", "vbap15_shorter_distance",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n1014(library, vbap14, 1014,
			"id_vbap_14", "vbap14",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1015(library, vbap15, 1015,
			"id_vbap_15", "vbap15",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1016(library, vbap16, 1016,
			"id_vbap_16", "vbap16",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1017(library, vbap17, 1017,
			"id_vbap_17", "vbap17",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1018(library, vbap18, 1018,
			"id_vbap_18", "vbap18",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1019(library, vbap19, 1019,
			"id_vbap_19", "vbap19",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1020(library, vbap20, 1020,
			"id_vbap_20", "vbap20",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1021(library, vbap21, 1021,
			"id_vbap_21", "vbap21",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n1022(library, vbap22, 1022,
			"id_vbap_22", "vbap22",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n2006(library, bformat06, 2006,
			"id_bformat_06", "bformat06",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2014(library, bformat14, 2014,
			"id_bformat_14", "bformat14",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2015(library, bformat15, 2015,
			"id_bformat_15", "bformat15",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2016(library, bformat16, 2016,
			"id_bformat_16", "bformat16",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2017(library, bformat17, 2017,
			"id_bformat_17", "bformat17",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2018(library, bformat18, 2018,
			"id_bformat_18", "bformat18",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2019(library, bformat19, 2019,
			"id_bformat_19", "bformat19",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2020(library, bformat20, 2020,
			"id_bformat_20", "bformat20",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2021(library, bformat21, 2021,
			"id_bformat_21", "bformat21",
			"BarcelonaMedia-Audio", "GNU GPL");
	static CLAM::LadspaNetworkExporter n2022(library, bformat22, 2022,
			"id_bformat_22", "bformat22",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n3015(library, decoded_bformat15, 3015,
			"id_decoded_bformat_15", "decoded_bformat15",
			"BarcelonaMedia-Audio", "GNU GPL");
	return library.pluginAt(index);
}
