
//#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"

#include "LADSPA_ProcessingBridge.hxx"

LADSPABridge* Instance(void)
{
	int id = 3001;
	return LADSPABridgeTmpl<CLAM::AudioMultiplier>::Create(id);
}

