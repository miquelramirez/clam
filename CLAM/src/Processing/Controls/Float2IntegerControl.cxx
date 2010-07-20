#include "Float2IntegerControl.hxx"
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Float2IntegerControl",
		"category", "Controls",
		"description", "Float2IntegerControl",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Float2IntegerControl> reg = metadata;
}

}
