#include "MagPhaseSpectrumProduct.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "MagPhaseSpectrumProduct",
		"category", "CLAM",
		"description", "MagPhaseSpectrumProduct",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrumProduct>
	reg = metadata;
}

}
