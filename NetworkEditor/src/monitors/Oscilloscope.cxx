#include "Oscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Oscilloscope",
		"category", "Monitors",
		"description", "Oscilloscope",
		"port_monitor_type", typeid(CLAM::TData).name(),
		"icon", "oscilloscope.svg",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, OscilloscopeMonitor> regOscilloscopeMonitor("Oscilloscope");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, OscilloscopeMonitor> reg = metadata;

/*	static class OscilloscopeMonitorMetadata
	{
	public:
		OscilloscopeMonitorMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Oscilloscope", "port_monitor_type", typeid(CLAM::TData).name());
			factory.AddAttribute("Oscilloscope", "icon", "oscilloscope.svg");
		}
	} dummy;*/
}

