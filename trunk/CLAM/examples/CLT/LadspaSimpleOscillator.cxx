
#include "Oscillator.hxx"
#include "LadspaBridge.hxx"

LadspaBridge* Instance(void)
{
	int id = 3000;
	return LadspaBridgeTmpl<CLAM::SimpleOscillator>::Create(id);
}

