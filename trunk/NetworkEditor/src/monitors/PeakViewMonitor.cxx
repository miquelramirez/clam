#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include "PeakViewMonitor.hxx"

namespace Hidden
{
	static const char * metadata[] = {
		"key", "PeakView",
		"category", "Graphical Monitors",
		"description", "PeakView",
		"port_monitor_type", typeid(CLAM::SpectralPeakArray).name(),
		"icon", "peakview.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PeakViewMonitor> reg = metadata;
}
