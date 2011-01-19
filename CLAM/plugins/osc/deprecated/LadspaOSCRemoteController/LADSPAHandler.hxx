#ifndef _LADSPA_HANDLER_
#define _LADSPA_HANDLER_

#include <ladspa.h>

extern "C"
{
	const LADSPA_Descriptor * ladspa_descriptor(unsigned long);
	LADSPA_Handle Instantiate(const LADSPA_Descriptor *, unsigned long);
	void Run(LADSPA_Handle, unsigned long);
	void CleanUp(LADSPA_Handle);
	void ConnectTo(LADSPA_Handle, unsigned long, LADSPA_Data *);
}

class StartupShutdownHandler
{
private:
	void CreateLADSPADescriptor();
public:
	StartupShutdownHandler();
	~StartupShutdownHandler();
};


#endif

