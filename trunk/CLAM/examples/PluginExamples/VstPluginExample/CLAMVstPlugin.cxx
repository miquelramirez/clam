#include <CLAM/EmbeddedFile.hxx>
#include "VstNetworkExporter.hxx"
#include <iostream>

CLAM_EMBEDDED_FILE(embededNetwork,"wire.xml")

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	std::cout << "== createEffectInstance" << std::endl;
	static CLAM::VstNetworkExporter exporter(
		embededNetwork,
		"CLAM effect",
		"Example of CLAM based VST plugin",
		"CLAM Project",
		1000
	);
	return exporter.createEffect(audioMaster);
}


