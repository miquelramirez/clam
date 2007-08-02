#ifndef MagPhaseSpectrum2Spectrum_hxx
#define MagPhaseSpectrum2Spectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Spectrum.hxx>
#include "MagPhaseSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class Spectrum2MagPhaseSpectrum : public Processing
{
	InPort<Spectrum> mSpectrum;
	OutPort<MagPhaseSpectrum> mMagPhaseSpectrum;
public:
	const char* GetClassName() const { return "Spectrum2MagPhaseSpectrum"; }
	Spectrum2MagPhaseSpectrum(const Config& config = Config() )
		: mSpectrum("Spectrum", this)
		, mMagPhaseSpectrum("MagPhaseSpectrum", this)
	{
		Configure( config );
	}
	bool Do()
	{
		const Spectrum & input = mSpectrum.GetData();
		MagPhaseSpectrum & output = mMagPhaseSpectrum.GetData();

		CLAM_ASSERT(input.HasMagBuffer(), "Input should have mag buffer");
		CLAM_ASSERT(input.HasPhaseBuffer(), "Input should have phase buffer");
		CLAM_ASSERT(input.GetScale()==EScale::eLinear, "Input scale should be linear");
		CLAM_ASSERT(input.GetPhaseBuffer().Size()==input.GetMagBuffer().Size(), 
			"Phase and magnitude buffers should be equal size");

		const unsigned nBins = input.GetSize(); 
		const TData* inMagnitude = input.GetMagBuffer().GetPtr();
		const TData* inPhase = input.GetPhaseBuffer().GetPtr();
		output.magnitudes.assign(inMagnitude, inMagnitude+nBins);
		output.phases.assign(inPhase, inPhase+nBins);
		output.spectralRange = input.GetSpectralRange();

		// Tell the ports this is done
		mSpectrum.Consume();
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
