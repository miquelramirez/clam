#include "Spectrum2MagPhaseSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "Spectrum2MagPhaseSpectrum",
		"category", "Spectral Processing",
		"description", "Spectrum2MagPhaseSpectrum",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Spectrum2MagPhaseSpectrum>
	reg = metadata;
}

}

