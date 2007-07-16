#ifndef ComplexSpectrumProduct_hxx
#define ComplexSpectrumProduct_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class ComplexSpectrumProduct : public Processing
{ 
	InPort<ComplexSpectrum> mFactor1;
	InPort<ComplexSpectrum> mFactor2;
	OutPort<ComplexSpectrum> mProduct;
public:
	const char* GetClassName() const { return "ComplexSpectrumProduct"; }
	ComplexSpectrumProduct(const Config& config = Config()) 
		: mFactor1("Factor1", this)
		, mFactor2("Factor2", this)
		, mProduct("Product", this) 
	{
		Configure( config );
	}
 
	bool Do()
	{
		const ComplexSpectrum & factor1 = mFactor1.GetData();
		const ComplexSpectrum & factor2 = mFactor2.GetData();
		ComplexSpectrum & product = mProduct.GetData();

		if ( factor1.bins.size()!=factor2.bins.size())
		{
			std::ostringstream os;
			os
				<< "Factors should be of equal size:"
				<< " Factor 1: " << factor1.bins.size()
				<< " Factor 2: " << factor2.bins.size()
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}
		CLAM_ASSERT(factor1.spectralRange==factor2.spectralRange, "SpectralRanges should be equal");

		product.spectralRange = factor1.spectralRange;
		const unsigned nBins = factor1.bins.size(); 
		product.bins.resize( nBins );
		for (unsigned i=0; i<nBins; i++)
		{
			product.bins[i] = factor1.bins[i] * factor2.bins[i];
		}

		// Tell the ports this is done
		mFactor1.Consume();
		mFactor2.Consume();
		mProduct.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ComplexSpectrumProduct_hxx
