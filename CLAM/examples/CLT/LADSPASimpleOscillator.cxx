
#include "Oscillator.hxx"
#include "LADSPA_ProcessingBridge.hxx"

LADSPABridge* Instance(void)
{
	int id = 3000;
	return LADSPABridgeTmpl<CLAM::SimpleOscillator>::Create(id);
}

