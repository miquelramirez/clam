#include <CLAM/ProcessingFactory.hxx>
#include "SpectrumViewMonitor.hxx"

namespace detail
{
	static const char * metadata[] = {
		"key", "SpectrumView",
		"category", "Graphical Monitors",
		"description", "SpectrumView",
		"port_monitor_type", typeid(CLAM::Spectrum).name(),
		"icon", "spectrumview.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, SpectrumViewMonitor> reg = metadata;

}
