#include <CLAM/PortMonitor.hxx>
#include <CLAM/Factory.hxx>

//TODO move to a clam lib

class SpectrumView : public CLAM::PortMonitor<Spectrum>
{
	const char* GetClassName() const { return "SpectrumView"; };
};

static CLAM::Factory<CLAM::Processing>::Registrator<SpectrumView> registrator("SpectrumView");


