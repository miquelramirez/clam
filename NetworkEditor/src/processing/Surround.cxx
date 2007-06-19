#include "Surround.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Surround> regSurround("Surround");
}
