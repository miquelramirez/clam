
#include "BFormatRotationZoom.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "BFormatRotationZoom",
		"category", "[plugin] 3D Audio",
		"description", "BFormatRotationZoom",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BFormatRotationZoom> reg = metadata;
}
