#include <CLAM/EmbeddedFile.hxx>
#include "VstNetworkExporter.hxx"
#include "QClamVstEditor.hxx"
#include <iostream>

CLAM_EMBEDDED_FILE(embededNetwork,"wire.xml")
CLAM_EMBEDDED_FILE(designerUI, "interface.ui")


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
	CLAM::VstNetworkExporter::Plugin * effect = exporter.createEffect(audioMaster);
	if (!effect) return 0;
	new QClamVstEditor(effect, designerUI);
	return effect;
}


