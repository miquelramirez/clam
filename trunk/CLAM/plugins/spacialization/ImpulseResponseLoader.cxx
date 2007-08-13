#include "ImpulseResponseLoader.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ImpulseResponseLoader",
		"category", "Spectral Processing",
		"description", "ImpulseResponseLoader",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseLoader>
	reg = metadata;
}

}
