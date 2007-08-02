#include "MagPhase2ComplexSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhase2ComplexSpectrum>
	regMagPhase2ComplexSpectrum("MagPhase2ComplexSpectrum");

	class regMagPhase2ComplexSpectrumMetadata
	{
	public:
		regMagPhase2ComplexSpectrumMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("MagPhase2ComplexSpectrum", "category", "CLAM");
			factory.AddAttribute("MagPhase2ComplexSpectrum", "description", "MagPhase2ComplexSpectrum");
		}
	};
	static regMagPhase2ComplexSpectrumMetadata dummy;

}

}

