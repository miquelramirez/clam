#include "ConstantSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{

static FactoryRegistrator<ProcessingFactory, ConstantSpectrum>
	regConstantSpectrum("ConstantSpectrum");

	class regConstantSpectrumMetadata
	{
	public:
		regConstantSpectrumMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("ConstantSpectrum", "category", "CLAM");
			factory.AddAttribute("ConstantSpectrum", "description", "MagPhaseSpectrum2Spectrum");
		}
	};
	static regConstantSpectrumMetadata dummy;
}
}

