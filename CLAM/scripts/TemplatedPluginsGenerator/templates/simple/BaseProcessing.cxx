#include "BaseProcessing.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BaseProcessing",
		"category", "Plugins",
		"description", "BaseProcessing",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BaseProcessing> reg = metadata;
}

}
