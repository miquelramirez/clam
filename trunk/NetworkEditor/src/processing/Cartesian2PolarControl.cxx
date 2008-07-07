#include "Cartesian2PolarControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Cartesian2PolarControl",
		"category", "3D Audio",
		"description", "Cartesian2PolarControl",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Cartesian2PolarControl> reg = metadata;
}
