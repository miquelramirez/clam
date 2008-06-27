#include <CLAM/ProcessingFactory.hxx>
#include "VumeterMonitor.hxx"

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Vumeter",
		"category", "Graphical Monitors",
		"description", "Vumeter",
		"port_monitor_type", typeid(CLAM::TData).name(),
		"icon", "vumeter.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VumeterMonitor> regVumeterMonitor=metadata;
}
