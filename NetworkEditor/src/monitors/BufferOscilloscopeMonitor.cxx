#include "BufferOscilloscopeMonitor.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BufferOscilloscope",
		"category", "Graphical Monitors",
		"description", "BufferOscilloscope",
		"port_monitor_type", typeid(CLAM::Audio).name(),
		"icon", "oscilloscope.svg",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BufferOscilloscopeMonitor> reg = metadata;

}
