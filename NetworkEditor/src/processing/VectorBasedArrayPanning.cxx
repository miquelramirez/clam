#include "VectorBasedArrayPanning.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VectorBasedArrayPanning> regVectorBasedArrayPanning("VectorBasedArrayPanning");
}
