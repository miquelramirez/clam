#include "ComplexSpectrumMixer.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ComplexSpectrumMixer>
	regComplexSpectrumMixer("ComplexSpectrumMixer");

	class regComplexSpectrumMixerMetadata
	{
	public:
		regComplexSpectrumMixerMetadata()
		{
			ProcessingFactory & factory = ProcessingFactory::GetInstance();
			factory.AddAttribute("ComplexSpectrumMixer", "category", "CLAM");
			factory.AddAttribute("ComplexSpectrumMixer", "description", "ComplexSpectrumMixer");
		}
	};
	static regComplexSpectrumMixerMetadata dummy;


}

}
