#include "Cartesian2PolarControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Cartesian2PolarControl",
	//	"category", "CLAM",
	//	"description", "Cartesian2PolarControl",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Cartesian2PolarControl> regCartesian2PolarControl("Cartesian2PolarControl");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Cartesian2PolarControl> reg = metadata;
}
