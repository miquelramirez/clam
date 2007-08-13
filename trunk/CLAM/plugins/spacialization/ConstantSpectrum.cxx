#include "ConstantSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ConstantSpectrum",
		"category", "CLAM",
		"description", "ConstantSpectrum",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ConstantSpectrum>
	reg = metadata;
}
}

