#include "BufferOscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BufferOscilloscope",
		"category", "Monitors",
		"description", "BufferOscilloscope",
		"port_monitor_type", typeid(CLAM::Audio).name(),
		"icon", "oscilloscope.svg",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BufferOscilloscopeMonitor> regBufferOscilloscopeMonitor("BufferOscilloscope");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BufferOscilloscopeMonitor> reg = metadata;

/*	static class BufferOscilloscopeMonitorMetadata
	{
	public:
		BufferOscilloscopeMonitorMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("BufferOscilloscope", "port_monitor_type", typeid(CLAM::Audio).name());
			factory.AddAttribute("BufferOscilloscope", "icon", "oscilloscope.svg");
		}
	} dummy;*/

}

