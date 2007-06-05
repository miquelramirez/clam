#include "Surround.hxx"
#include <CLAM/Factory.hxx>

namespace detail
{
	static CLAM::Factory<CLAM::Processing>::Registrator<Surround>
		myProcessingRegistrator("Surround");
}
