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

} // namespace CLAM

#endif 
