#include "MagPhaseSpectrum2Spectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrum2Spectrum>
	regMagPhaseSpectrum2Spectrum("MagPhaseSpectrum2Spectrum");

	class regMagPhaseSpectrum2SpectrumMetadata
	{
	public:
		regMagPhaseSpectrum2SpectrumMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("MagPhaseSpectrum2Spectrum", "category", "CLAM");
			factory.AddAttribute("MagPhaseSpectrum2Spectrum", "description", "MagPhaseSpectrum2Spectrum");
		}
	};
	static regMagPhaseSpectrum2SpectrumMetadata dummy;

}

}

