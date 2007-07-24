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
	InPort<ComplexSpectrum> mTerm1;
	InPort<ComplexSpectrum> mTerm2;
	OutPort<ComplexSpectrum> mSum;
public:
	const char* GetClassName() const { return "ComplexSpectrumSum"; }
	ComplexSpectrumSum(const Config& config = Config()) 
		: mTerm1("Term1", this)
		, mTerm2("Term2", this)
		, mSum("Sum", this) 
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
		const ComplexSpectrum & term1 = mTerm1.GetData();
		const ComplexSpectrum & term2 = mTerm2.GetData();
		ComplexSpectrum & sum = mSum.GetData();
		Do(term1, term2, sum);

		// Tell the ports this is done
		mTerm1.Consume();
		mTerm2.Consume();
		mSum.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ComplexSpectrumSum_hxx
