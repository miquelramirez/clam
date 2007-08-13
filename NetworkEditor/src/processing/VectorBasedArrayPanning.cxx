#include "VectorBasedArrayPanning.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "VectorBasedArrayPanning",
		"category", "CLAM",
		"description", "VectorBasedArrayPanning",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorBasedArrayPanning> regVectorBasedArrayPanning("VectorBasedArrayPanning");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorBasedArrayPanning> reg = metadata;
}
