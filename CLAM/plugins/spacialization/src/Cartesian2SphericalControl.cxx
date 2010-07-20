#include "Cartesian2SphericalControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Cartesian2SphericalControl",
		"category", "Controls",
		"description", "Cartesian2SphericalControl",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Cartesian2SphericalControl> reg = metadata;
}
