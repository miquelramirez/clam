#include "DebugImpulseResponse.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "DebugImpulseResponse",
		"category", "Spectral Processing",
		"description", "DebugImpulseResponse",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, DebugImpulseResponse>
	reg = metadata;
}

}
