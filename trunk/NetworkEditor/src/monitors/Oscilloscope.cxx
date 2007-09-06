#include "Oscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Oscilloscope",
		"category", "Graphical Monitors",
		"description", "Oscilloscope",
		"port_monitor_type", typeid(CLAM::TData).name(),
		"icon", "oscilloscope.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, OscilloscopeMonitor> reg = metadata;
}

