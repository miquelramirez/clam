#include "Oscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, OscilloscopeMonitor> regOscilloscopeMonitor("Oscilloscope");
	static class OscilloscopeMonitorMetadata
	{
	public:
		OscilloscopeMonitorMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Oscilloscope", "port_monitor_type", typeid(CLAM::TData).name());
		}
	} dummy;
}

