#include "LowLatencyConvolution.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, LowLatencyConvolution>
	regLowLatencyConvolution("LowLatencyConvolution");
	class regLowLatencyConvolutionMetadata
	{
	public:
		regLowLatencyConvolutionMetadata()
		{
			ProcessingFactory & factory = ProcessingFactory::GetInstance();
			factory.AddAttribute("LowLatencyConvolution", "category", "CLAM");
			factory.AddAttribute("LowLatencyConvolution", "description", "LowLatencyConvolution");
		}
	};
	static regLowLatencyConvolutionMetadata dummy;



}

}
