#ifndef MagPhaseSpectrum2Spectrum_hxx
#define MagPhaseSpectrum2Spectrum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Spectrum.hxx>
#include <CLAM/SpecTypeFlags.hxx>
#include "MagPhaseSpectrum.hxx"

namespace CLAM
{

class MagPhaseSpectrum2Spectrum : public Processing
{
	InPort<MagPhaseSpectrum> mMagPhaseSpectrum;
	OutPort<Spectrum> mSpectrum;
public:
	const char* GetClassName() const { return "MagPhaseSpectrum2Spectrum"; }
	MagPhaseSpectrum2Spectrum(const Config& config = Config() )
		: mMagPhaseSpectrum("MagPhaseSpectrum", this)
		, mSpectrum("Spectrum", this)
	{
		Configure( config );
	}
	bool Do()
	{
		const MagPhaseSpectrum & input = mMagPhaseSpectrum.GetData();
		Spectrum & output = mSpectrum.GetData();

		CLAM_ASSERT(input.magnitudes.size()==input.phases.size(), 
			"Phase and magnitude buffers should be equal size");
		
		const unsigned nBins = input.magnitudes.size(); 
		SpecTypeFlags type;
		type.bMagPhase = true;
		output.SetType( type );
		output.SetSize( nBins );
		output.SetSpectralRange( input.spectralRange );
		output.SetScale( EScale::eLinear );

		TData* outMagBuffer = output.GetMagBuffer().GetPtr();
		TData* outPhaseBuffer = output.GetPhaseBuffer().GetPtr();
		std::copy( input.magnitudes.begin(), input.magnitudes.end(), outMagBuffer );
		std::copy( input.phases.begin(), input.phases.end(), outPhaseBuffer );

		// Tell the ports this is done
		mMagPhaseSpectrum.Consume();
		mSpectrum.Produce();

		return true;
	}
};

} // namespace CLAM

#endif 
