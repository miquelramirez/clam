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
		"control_sender_type", typeid(bool).name(),
		0
	};
	static FactoryRegistrator<ProcessingFactory, BoolControlSender> reg = metadata;
}

}
