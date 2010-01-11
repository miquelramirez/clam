#include "BoolControlPrinter.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BoolControlPrinter",
		"category", "Controls",
		"description", "BoolControlPrinter",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BoolControlPrinter> reg = metadata;
}

}
