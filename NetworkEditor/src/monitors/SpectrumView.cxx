#include <CLAM/ProcessingFactory.hxx>
#include "SpectrumView.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrumViewMonitor> regSpectrumViewMonitor("SpectrumView");
}

