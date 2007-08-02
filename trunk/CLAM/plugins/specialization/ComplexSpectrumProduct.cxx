#include "ComplexSpectrumProduct.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ComplexSpectrumProduct>
	regComplexSpectrumProduct("ComplexSpectrumProduct");

	class regComplexSpectrumProductMetadata
	{
	public:
		regComplexSpectrumProductMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("ComplexSpectrumProduct", "category", "CLAM");
			factory.AddAttribute("ComplexSpectrumProduct", "description", "ComplexSpectrumProduct");
		}
	};
	static regComplexSpectrumProductMetadata dummy;



}

}
