#include "VectorBasedArrayPanning.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "VectorBasedArrayPanning",
		"category", "[plugin] 3D Audio",
		"description", "VectorBasedArrayPanning",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorBasedArrayPanning> reg = metadata;
}
