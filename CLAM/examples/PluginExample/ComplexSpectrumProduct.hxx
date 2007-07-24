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
	InPort<ComplexSpectrum> _factor1;
	InPort<ComplexSpectrum> _factor2;
	OutPort<ComplexSpectrum> _product;
public:
	const char* GetClassName() const { return "ComplexSpectrumProduct"; }
	ComplexSpectrumProduct(const Config& config = Config()) 
		: _factor1("Factor1", this)
		, _factor2("Factor2", this)
		, _product("Product", this) 
	{
		Configure( config );
	}
 
	bool Do(const ComplexSpectrum & in1, const ComplexSpectrum & in2, ComplexSpectrum & out)
	{
		if ( in1.bins.size()!=in2.bins.size())
		{
			std::ostringstream os;
			os
				<< "Factors should be of equal size:"
				<< " Factor 1: " << in1.bins.size()
				<< " Factor 2: " << in2.bins.size()
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}
		CLAM_ASSERT(in1.spectralRange==in2.spectralRange, "SpectralRanges should be equal");

		out.spectralRange = in1.spectralRange;
		const unsigned nBins = in1.bins.size(); 
		out.bins.resize( nBins );
		for (unsigned i=0; i<nBins; i++)
		{
			out.bins[i] = in1.bins[i] * in2.bins[i];
		}
		return true;
	}
	bool Do()
	{
		const ComplexSpectrum & factor1 = _factor1.GetData();
		const ComplexSpectrum & factor2 = _factor2.GetData();
		ComplexSpectrum & product = _product.GetData();

		Do(factor1, factor2, product);

		// Tell the ports this is done
		_factor1.Consume();
		_factor2.Consume();
		_product.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ComplexSpectrumProduct_hxx
