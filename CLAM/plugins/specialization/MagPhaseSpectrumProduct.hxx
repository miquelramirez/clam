#ifndef MagPhaseSpectrumProduct_hxx
#define MagPhaseSpectrumProduct_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "MagPhaseSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class MagPhaseSpectrumProduct : public Processing
{ 
	InPort<MagPhaseSpectrum> _factor1;
	InPort<MagPhaseSpectrum> _factor2;
	OutPort<MagPhaseSpectrum> _product;
public:
	const char* GetClassName() const { return "MagPhaseSpectrumProduct"; }
	MagPhaseSpectrumProduct(const Config& config = Config()) 
		: _factor1("Factor1", this)
		, _factor2("Factor2", this)
		, _product("Product", this) 
	{
		Configure( config );
	}

	bool Do()
	{
		const MagPhaseSpectrum & factor1 = _factor1.GetData();
		const MagPhaseSpectrum & factor2 = _factor2.GetData();
		MagPhaseSpectrum & product = _product.GetData();

		if ( factor1.magnitudes.size()!=factor2.magnitudes.size())
		{
			std::ostringstream os;
			os
				<< "Factors should be of equal size:"
				<< " Factor 1: " << factor1.magnitudes.size()
				<< " Factor 2: " << factor2.magnitudes.size()
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}
		CLAM_ASSERT(factor1.spectralRange==factor2.spectralRange, "SpectralRanges should be equal");

		product.spectralRange = factor1.spectralRange;
		const unsigned nBins = factor1.magnitudes.size(); 
		product.magnitudes.resize( nBins );
		product.phases.resize( nBins );
		for (unsigned i=0; i<nBins; i++)
		{
			product.magnitudes[i] = factor1.magnitudes[i] * factor2.magnitudes[i];
			product.phases[i] = factor1.phases[i] + factor2.phases[i];
		}

		// Tell the ports this is done
		_factor1.Consume();
		_factor2.Consume();
		_product.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // MagPhaseSpectrumProduct_hxx
