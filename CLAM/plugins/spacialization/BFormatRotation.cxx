
#include "BFormatRotation.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "BFormatRotation",
		"category", "[plugin] 3D Audio",
		"description", "BFormatRotation",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, BFormatRotation> reg = metadata;
}
