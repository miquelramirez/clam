#ifndef MagPhase2ComplexSpectrum_hxx
#define MagPhase2ComplexSpectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "MagPhaseSpectrum.hxx"
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class MagPhase2ComplexSpectrum : public Processing
{
	InPort<MagPhaseSpectrum> mMagPhaseSpectrum;
	OutPort<ComplexSpectrum> mComplexSpectrum;
public:
	const char* GetClassName() const { return "MagPhase2ComplexSpectrum"; }
	MagPhase2ComplexSpectrum(const Config& config = Config() )
		: mMagPhaseSpectrum("MagPhaseSpectrum", this)
		, mComplexSpectrum("ComplexSpectrum", this)
	{
		Configure( config );
	}
	bool Do()
	{
		const MagPhaseSpectrum & input = mMagPhaseSpectrum.GetData();
		ComplexSpectrum & output = mComplexSpectrum.GetData();

		const unsigned nBins = input.magnitudes.size();
		output.spectralRange = input.spectralRange;
		output.bins.resize(nBins);
		for (unsigned i = 0; i<nBins; i++)
		{
			output.bins[i] = std::polar(input.magnitudes[i], input.phases[i]);
		}

		// Tell the ports this is done
		mMagPhaseSpectrum.Consume();
		mComplexSpectrum.Produce();

		return true;
	}
};


} // namespace CLAM

#endif 
