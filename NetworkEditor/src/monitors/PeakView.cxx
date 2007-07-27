
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include "PeakView.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PeakViewMonitor> regPeakViewMonitor("PeakView");
	static class PeakViewMetadata
	{
	public:
		PeakViewMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("PeakView", "port_monitor_type", typeid(CLAM::SpectralPeakArray).name());
			factory.AddAttribute("PeakView", "icon", "peakview.svg");
		}
	} dummy;
}

