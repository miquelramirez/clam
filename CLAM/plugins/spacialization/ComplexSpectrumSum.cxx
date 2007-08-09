#include "ComplexSpectrumSum.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ComplexSpectrumSum>
	regComplexSpectrumSum("ComplexSpectrumSum");

	class regComplexSpectrumSumMetadata
	{
	public:
		regComplexSpectrumSumMetadata()
		{
			ProcessingFactory & factory = ProcessingFactory::GetInstance();
			factory.AddAttribute("ComplexSpectrumSum", "category", "CLAM");
			factory.AddAttribute("ComplexSpectrumSum", "description", "ComplexSpectrumSum");
		}
	};
	static regComplexSpectrumSumMetadata dummy;


}

}
