#ifndef ComplexSpectrumSum_hxx
#define ComplexSpectrumSum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class ComplexSpectrumSum : public Processing
{ 
	InPort<ComplexSpectrum> _term1;
	InPort<ComplexSpectrum> _term2;
	OutPort<ComplexSpectrum> _sum;
public:
	const char* GetClassName() const { return "ComplexSpectrumSum"; }
	ComplexSpectrumSum(const Config& config = Config()) 
		: _term1("Term1", this)
		, _term2("Term2", this)
		, _sum("Sum", this) 
	{
		Configure( config );
	}

	bool Do(const ComplexSpectrum & in1, const ComplexSpectrum & in2, ComplexSpectrum & out)
	{
		if ( in1.bins.size()!=in2.bins.size())
		{
			std::ostringstream os;
			os
				<< "Operands should be of equal size:"
				<< " Operand 1: " << in1.bins.size()
				<< " Operand 2: " << in2.bins.size()
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}
		CLAM_ASSERT(in1.spectralRange==in2.spectralRange, "SpectralRanges should be equal");

		out.spectralRange = in1.spectralRange;
		const unsigned nBins = in1.bins.size(); 
		out.bins.resize( nBins );
		for (unsigned i=0; i<nBins; i++)
		{
			out.bins[i] = in1.bins[i] + in2.bins[i];
		}
		return true;
	}
	bool Do()
	{
		const ComplexSpectrum & term1 = _term1.GetData();
		const ComplexSpectrum & term2 = _term2.GetData();
		ComplexSpectrum & sum = _sum.GetData();

		Do(term1, term2, sum);

		// Tell the ports this is done
		_term1.Consume();
		_term2.Consume();
		_sum.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ComplexSpectrumSum_hxx
