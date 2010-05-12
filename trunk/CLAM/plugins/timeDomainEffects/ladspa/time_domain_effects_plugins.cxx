
#include <CLAM/LadspaNetworkExporter.hxx>
#include <CLAM/LadspaLibrary.hxx>
#include <CLAM/EmbeddedFile.hxx>




CLAM_EMBEDDED_FILE(doppler_distance,"LadspaDopplerAndDistance.clamnetwork")
CLAM_EMBEDDED_FILE(doppler,"LadspaDoppler.clamnetwork")
CLAM_EMBEDDED_FILE(distanceAttenuation,"LadspaDistance.clamnetwork")


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

	static CLAM::LadspaNetworkExporter n4003(library, distanceAttenuation, 4004,
			"distanceAttenuation", "distanceAtten for a moving source",
			"BarcelonaMedia-Audio", "GNU GPL");

	return library.pluginAt(index);
}
