/*
 * Copyright (c) 2007 Fundació Barcelona Media Universitat Pompeu Fabra
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

/**
 This processing converts an spectrum in
 Magnitude/Phase representation (MagPhaseSpectrum)
 into a deprecated CLAM::Spectrum.

 @param[in] MagPhaseSpectrum [Port] A magnitude-phase spectrum.
 @param[out] Spectrum [Port] A deprecated version of CLAM::Spectrum.

 The NewSpectralProcessing module is the one to substitute actual
 spectral processing classes in CLAM on the long term.
 Meanwhile, this processing is provided for convenience to make both
 world compatible.

 @see ComplexSpectrum, MagPhaseSpectrum, Spectrum2MagPhaseSpectrum
 @ingroup SpectralProcessingConversions
*/
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
