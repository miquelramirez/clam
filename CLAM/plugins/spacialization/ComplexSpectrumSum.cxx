#include "ComplexSpectrumSum.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ComplexSpectrumSum",
		"category", "CLAM",
		"description", "ComplexSpectrumSum",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ComplexSpectrumSum>
	reg = metadata;
}

}
