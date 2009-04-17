#include "VumeterRMSPrinter.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/TypeInfo.hxx>

namespace detail
{
	static const char * metadata[] = {
		"key", "VumeterRMSPrinter",
		"category", "Analysis",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::VumeterRMSPrinter> reg = metadata;
}


