
#include "AudioMultiplier.hxx"

#include "LadspaBridge.hxx"

LadspaBridge* Instance(void)
{
	int id = 3001;
	return LadspaBridgeTmpl<CLAM::AudioMultiplier>::Create(id);
}

