#include "Complex2MagPhaseSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Complex2MagPhaseSpectrum>
	regComplex2MagPhaseSpectrum("Complex2MagPhaseSpectrum");

	class regComplex2MagPhaseSpectrumMetadata
	{
	public:
		regComplex2MagPhaseSpectrumMetadata()
		{
			CLAM::ProcessingFactory & factory = CLAM::ProcessingFactory::GetInstance();
			factory.AddAttribute("Complex2MagPhaseSpectrum", "category", "CLAM");
			factory.AddAttribute("Complex2MagPhaseSpectrum", "description", "Complex2MagPhaseSpectrum");
		}
	};
	static regComplex2MagPhaseSpectrumMetadata dummy;

}

}

