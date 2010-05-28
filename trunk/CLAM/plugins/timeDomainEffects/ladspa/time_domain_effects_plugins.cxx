
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
#include <CLAM/EmbeddedFile.hxx>




CLAM_EMBEDDED_FILE(doppler_distance,"LadspaDopplerAndDistance.clamnetwork")
CLAM_EMBEDDED_FILE(doppler,"LadspaDoppler.clamnetwork")
CLAM_EMBEDDED_FILE(distanceAttenuation,"LadspaDistance.clamnetwork")
CLAM_EMBEDDED_FILE(chorus_15,"3d_chorus_15.clamnetwork")
CLAM_EMBEDDED_FILE(chorus_22,"3d_chorus_22.clamnetwork")
CLAM_EMBEDDED_FILE(rotatingFlanger_22,"LadspaRotatingFlanger.clamnetwork")


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

	static CLAM::LadspaNetworkExporter n4001(library, doppler_distance, 4001,
			"doppler_distance", "doppler and distanceAtten for a moving source",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n4002(library, doppler, 4002,
			"doppler", "doppler for a moving source",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n4003(library, distanceAttenuation, 4003,
			"distanceAttenuation", "distanceAtten for a moving source",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n4004(library, chorus_15, 4004,
			"chorus_15", "3d chorus_15",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n4005(library, chorus_22, 4005,
			"chorus_22", "3d chorus_22",
			"BarcelonaMedia-Audio", "GNU GPL");

	static CLAM::LadspaNetworkExporter n4006(library, rotatingFlanger_22, 4006,
			"rotatingFlanger_22", "3d rotating Flanger",
			"BarcelonaMedia-Audio", "GNU GPL");


	return library.pluginAt(index);
}
