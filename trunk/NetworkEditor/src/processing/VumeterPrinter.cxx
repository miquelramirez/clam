#include "VumeterPrinter.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/TypeInfo.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "VumeterPrinter",
		"category", "Analysis",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::VumeterPrinter> reg = metadata;
}


