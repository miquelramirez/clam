
#include <CLAM/ProcessingFactory.hxx>
#include "PeakView.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PeakViewMonitor> regPeakViewMonitor("PeakView");
}

