
#include <CLAM/PortMonitor.hxx>
#include <CLAM/Factory.hxx>
#include <CLAM/SpectralPeakArray.hxx>


//TODO move to a clam lib

class PeakView : public CLAM::PortMonitor<CLAM::SpectralPeakArray>
{
	const char* GetClassName() const { return "PeakView"; };
};

static CLAM::Factory<CLAM::Processing>::Registrator<PeakView> registrator("PeakView");


