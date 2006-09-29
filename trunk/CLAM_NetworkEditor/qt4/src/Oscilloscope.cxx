
#include <CLAM/PortMonitor.hxx>
#include <CLAM/Factory.hxx>

//TODO move to a clam lib

class Oscilloscope : public CLAM::AudioPortMonitor
{
	const char* GetClassName() const { return "Oscilloscope"; };
};
static CLAM::Factory<CLAM::Processing>::Registrator<Oscilloscope> registrator("Oscilloscope");


