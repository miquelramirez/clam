#include "MagPhase2ComplexSpectrum.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{

namespace Hidden
{
	static const char* metadata [] = {
		"key", "MagPhase2ComplexSpectrum",
		"category", "CLAM",
		"description", "MagPhase2ComplexSpectrum",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhase2ComplexSpectrum>
	reg = metadata;
}

}

