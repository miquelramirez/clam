#ifndef _LADSPA_HANDLER_
#define _LADSPA_HANDLER_

#include <ladspa.h>

class StartupShutdownHandler
{
private:
	void CreateLADSPADescriptor();
public:
	StartupShutdownHandler();
	~StartupShutdownHandler();
};


#endif

