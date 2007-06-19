#include "Oscilloscope.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, OscilloscopeMonitor> regOscilloscopeMonitor("Oscilloscope");
}

