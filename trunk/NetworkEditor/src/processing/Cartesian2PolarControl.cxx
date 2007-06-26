#include "Cartesian2PolarControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Cartesian2PolarControl> regCartesian2PolarControl("Cartesian2PolarControl");
}
