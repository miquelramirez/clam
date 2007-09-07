#include "ImpulseResponseInterpolator.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ImpulseResponseInterpolator",
		"category", "Spectral Processing",
		"description", "ImpulseResponseInterpolator",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseInterpolator>
	reg = metadata;
}

}
