#include "BoolControlSender.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BoolControlSender",
		"category", "Controls",
		"description", "BoolControlSender",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BoolControlSender> reg = metadata;
}

}
