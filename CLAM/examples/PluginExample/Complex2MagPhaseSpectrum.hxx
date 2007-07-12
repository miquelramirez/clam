#ifndef Complex2MagPhaseSpectrum_hxx
#define Complex2MagPhaseSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include "MagPhaseSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class Complex2MagPhaseSpectrum : public Processing
{
	InPort<ComplexSpectrum> mComplexSpectrum;
	OutPort<MagPhaseSpectrum> mMagPhaseSpectrum;
public:
	const char* GetClassName() const { return "Complex2MagPhaseSpectrum"; }
	Complex2MagPhaseSpectrum(const Config& config = Config() )
		: mComplexSpectrum("ComplexSpectrum", this)
		, mMagPhaseSpectrum("MagPhaseSpectrum", this)
	{
		Configure( config );
	}
	bool Do()
	{
		const ComplexSpectrum & input = mComplexSpectrum.GetData();
		MagPhaseSpectrum & output = mMagPhaseSpectrum.GetData();

		const unsigned nBins = input.bins.size();
		output.spectralRange = input.spectralRange;
		output.magnitudes.resize(nBins);
		output.phases.resize(nBins);
		for (unsigned i = 0; i<nBins; i++)
		{
			output.magnitudes[i] = std::abs(input.bins[i]);
			output.phases[i] = std::arg(input.bins[i]);
		}

		// Tell the ports this is done
		mComplexSpectrum.Consume();
		mMagPhaseSpectrum.Produce();

		return true;
	}
};

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
	
		CLAM_ASSERT(factor1.magnitudes.size()==factor2.magnitudes.size(), "Factors should be of equal size");
		CLAM_ASSERT(factor1.spectralRange==factor2.spectralRange, "SpectralRanges should be equal");

		product.spectralRange = factor1.spectralRange;
		const unsigned nBins = factor1.magnitudes.size(); 
//		product.magnitudes.reserve( nBins );
//		product.phases.reserve( nBins );
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

#endif 
