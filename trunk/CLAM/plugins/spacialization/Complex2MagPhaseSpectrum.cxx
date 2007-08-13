#include "Complex2MagPhaseSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char* metadata[] = {
		"key", "Complex2MagPhaseSpectrum",
		"category", "Spectral Processing",
		"description", "Complex2MagPhaseSpectrum",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Complex2MagPhaseSpectrum>
		reg = metadata;
}

}

