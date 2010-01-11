#include "BinaryCounter.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "BinaryCounter",
		"category", "Controls",
		"description", "BinaryCounter",
		0
	};
	static FactoryRegistrator<ProcessingFactory, BinaryCounter> reg = metadata;
}

}
