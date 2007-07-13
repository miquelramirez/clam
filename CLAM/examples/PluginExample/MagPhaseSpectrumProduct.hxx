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
	InPort<MagPhaseSpectrum> mFactor1;
	InPort<MagPhaseSpectrum> mFactor2;
	OutPort<MagPhaseSpectrum> mProduct;
public:
	const char* GetClassName() const { return "MagPhaseSpectrumProduct"; }
	MagPhaseSpectrumProduct(const Config& config = Config()) 
		: mFactor1("Factor1", this)
		, mFactor2("Factor2", this)
		, mProduct("Product", this) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		const MagPhaseSpectrum & factor1 = mFactor1.GetData();
		const MagPhaseSpectrum & factor2 = mFactor2.GetData();
		MagPhaseSpectrum & product = mProduct.GetData();

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
		mFactor1.Consume();
		mFactor2.Consume();
		mProduct.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // MagPhaseSpectrumProduct_hxx
