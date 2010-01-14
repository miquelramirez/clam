#include "ControlGate.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlGate",
		"category", "Controls",
		"description", "ControlGate",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlGate> reg = metadata;
}

}
