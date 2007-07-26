#include "Spectrum2MagPhaseSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Spectrum2MagPhaseSpectrum>
	regSpectrum2MagPhaseSpectrum("Spectrum2MagPhaseSpectrum");

	class regSpectrum2MagPhaseSpectrumMetadata
	{
	public:
		regSpectrum2MagPhaseSpectrumMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Spectrum2MagPhaseSpectrum", "category", "CLAM");
			factory.AddAttribute("Spectrum2MagPhaseSpectrum", "description", "Spectrum2MagPhaseSpectrum: Conversion between old Spectrum and new MagPhaseSpectrum");
		}
	};
static regSpectrum2MagPhaseSpectrumMetadata dummy;

}

}

