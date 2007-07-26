#include "MagPhaseSpectrumProduct.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrumProduct>
	regMagPhaseSpectrumProduct("MagPhaseSpectrumProduct");

	class regMagPhaseSpectrumProductMetadata
	{
	public:
		regMagPhaseSpectrumProductMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("MagPhaseSpectrumProduct", "category", "CLAM");
			factory.AddAttribute("MagPhaseSpectrumProduct", "description", "MagPhaseSpectrumProduct");
		}
	};
	static regMagPhaseSpectrumProductMetadata dummy;



}

}
