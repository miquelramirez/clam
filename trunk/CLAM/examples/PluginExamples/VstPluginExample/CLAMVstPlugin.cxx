#include <CLAM/EmbeddedFile.hxx>
#include "VstNetworkExporter.hxx"
#include "QClamVstEditor.hxx"
#include <iostream>

//CLAM_EMBEDDED_FILE(embededNetwork,"wire.xml")
//CLAM_EMBEDDED_FILE(embededUI, "interface.ui")
extern char embededUI[];
extern char embededNetwork[];

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	static CLAM::VstNetworkExporter exporter(
		audioMaster,
		embededNetwork,
		CCONST('C','L','A','M'),
		"CLAM effect", // Effect name
		"Example of CLAM based VST plugin", // Product string
		"CLAM Project", // Vendor
		10 // Version
	);
	CLAM::VstNetworkExporter::Plugin * effect = exporter.createEffect(audioMaster);
	if (!effect) return 0;
	new QClamVstEditor(effect, embededUI);
	return effect;
}


