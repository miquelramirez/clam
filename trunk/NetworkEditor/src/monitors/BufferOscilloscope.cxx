#include "BufferOscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BufferOscilloscopeMonitor> regBufferOscilloscopeMonitor("BufferOscilloscope");
}

