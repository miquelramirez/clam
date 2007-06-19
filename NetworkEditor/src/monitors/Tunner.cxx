
#include <CLAM/ProcessingFactory.hxx>
#include "Tunner.hxx"

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, TunnerMonitor> regTunnerMonitor("Tunner");
}

