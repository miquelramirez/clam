#include "Spherical2CartesianControl.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Spherical2CartesianControl",
		"category", "Controls",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Spherical2CartesianControl> reg = metadata;
}
