#include <CLAM/PortMonitor.hxx>
#include <CLAM/Factory.hxx>

//TODO move to a clam lib

class Vumeter : public CLAM::AudioPortMonitor
{
	const char* GetClassName() const { return "Vumeter"; };
};

static CLAM::Factory<CLAM::Processing>::Registrator<Vumeter> registrator("Vumeter");


