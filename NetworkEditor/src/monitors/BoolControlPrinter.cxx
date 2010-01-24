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
		"control_display_type", typeid(bool).name(),
		"icon", "ledon1.png",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BoolControlPrinter> reg = metadata;
}

}
