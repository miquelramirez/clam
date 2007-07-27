#include "BufferOscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BufferOscilloscopeMonitor> regBufferOscilloscopeMonitor("BufferOscilloscope");

	static class BufferOscilloscopeMonitorMetadata
	{
	public:
		BufferOscilloscopeMonitorMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("BufferOscilloscope", "port_monitor_type", typeid(CLAM::Audio).name());
			factory.AddAttribute("BufferOscilloscope", "icon", "oscilloscope.svg");
		}
	} dummy;

}

