#include "ImpulseResponseLoaderAndInterpolator.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ImpulseResponseLoaderAndInterpolator",
		"category", "CLAM",
		"description", "ImpulseResponseLoaderAndInterpolator",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ImpulseResponseLoaderAndInterpolator>
	reg = metadata;
}

}
