#include <CLAM/EmbeddedFile.hxx>
#include "VstNetworkExporter.hxx"
#include "QClamVstEditor.hxx"
#include <iostream>
#include <CLAM/CLAM_windows.h>

#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/SimpleOscillator.hxx>

// TODO: This cannot be used until mingw is fixed. See 'Files.s'
//CLAM_EMBEDDED_FILE(embededNetwork,"wire.xml")
//CLAM_EMBEDDED_FILE(embededUI, "interface.ui")
extern const char embededUI[];
extern const char embededNetwork[];

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	CLAM::SimpleOscillator osc; // To load clam_processing
	CLAM::MonoAudioFileReader reader; // To load clam_audioio
	
	static CLAM::VstNetworkExporter exporter(
		embededNetwork,
		CCONST('C','L','A','M'),
		"CLAM effect", // Effect name
		"Example of CLAM based VST plugin", // Product string
		"CLAM Project", // Vendor
		0x0100 // Version
	);
	CLAM::VstNetworkExporter::Plugin * effect = exporter.createEffect(audioMaster);
	if (effect==0) return 0;
	new QClamVstEditor(effect, embededUI);
	return effect;
}


