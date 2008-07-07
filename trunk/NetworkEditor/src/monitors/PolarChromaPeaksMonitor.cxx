#include <CLAM/ProcessingFactory.hxx>
#include "PolarChromaPeaksMonitor.hxx"

namespace Hidden
{
	static const char * metadata[] = {
		"key", "PolarChromaPeaks",
		"category", "Graphical Monitors",
		"description", "PolarChromaPeaks",
		"port_monitor_type", typeid(std::vector<std::pair<CLAM::TData, CLAM::TData> >).name(),
		"icon", "polarchromapeaks.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, PolarChromaPeaksMonitor> reg = metadata;
}
