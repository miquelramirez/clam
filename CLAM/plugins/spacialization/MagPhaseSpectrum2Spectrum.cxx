#include "MagPhaseSpectrum2Spectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "MagPhaseSpectrum2Spectrum",
		"category", "CLAM",
		"description", "MagPhaseSpectrum2Spectrum",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrum2Spectrum>
	reg = metadata;
}

}

