#include "MagPhaseSpectrumProduct.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace detail
{

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrumProduct>
	regMagPhaseSpectrumProduct("MagPhaseSpectrumProduct");

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, Spectrum2MagPhaseSpectrum>
	regSpectrum2MagPhaseSpectrum("Spectrum2MagPhaseSpectrum");

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, MagPhaseSpectrum2Spectrum>
	regMagPhaseSpectrum2Spectrum("MagPhaseSpectrum2Spectrum");

}

}
