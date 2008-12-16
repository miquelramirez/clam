#include "BFormat2Cube.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "BFormat2Cube",
		"category", "[plugin] 3D Audio",
		"description", "BFormat to 8 speakers in a cube",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BFormat2Cube> reg = metadata;
}
