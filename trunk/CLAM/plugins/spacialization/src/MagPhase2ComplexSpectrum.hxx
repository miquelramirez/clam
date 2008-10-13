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

/**
 This processing converts an spectrum in
	Magnitude/Phase representation (MagPhaseSpectrum)
 into one which uses 
	Complex representation (ComplexSpectrum).
 Linear values are suposed in both representations.

 @param[in] MagPhaseSpectrum [Port] A magnitude-phase
 @param[out] ComplexSpectrum [Port] A complex spectrum.

 Magnitude-Phase representation is more convenient for some operations.
 Anyway, as the FFT and IFFT works with complex form, consider with a profiler
 whether the cost of the conversion is worth for the cost you save.

 @see ComplexSpectrum, MagPhaseSpectrum, Complex2MagPhaseSpectrum
 @ingroup NewSpectralProcessing
*/
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
