#include "Surround.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "Surround",
	//	"category", "CLAM",
	//	"description", "Surround",
		0
	};
	//static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Surround> regSurround("Surround");
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Surround> reg = metadata;
}
