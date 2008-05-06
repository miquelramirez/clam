
#include <CLAM/ProcessingFactory.hxx>
#include "Tunner.hxx"

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Tunner",
		"category", "Graphical Monitors",
		"description", "Tunner",
		"port_monitor_type", typeid(std::pair<CLAM::TData,CLAM::TData>).name(),
		"icon", "tunner.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, TunnerMonitor> reg = metadata;
}

