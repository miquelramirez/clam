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

/**
 This processing converts an spectrum in
	Complex representation (ComplexSpectrum)
 into one which uses 
	Magnitude/Phase representation (MagPhaseSpectrum).
 Linear values are suposed in both representations.

 @param[in] ComplexSpectrum [Port] A complex spectrum.
 @param[out] MagPhaseSpectrum [Port] A magnitude-phase

 Magnitude-Phase representation is more convenient for some operations.
 Anyway, as the FFT and IFFT works with complex form, consider with a profiler
 whether the cost of the conversion is worth for the cost you save.

 @see ComplexSpectrum, MagPhaseSpectrum, MagPhase2ComplexSpectrum
 @ingroup NewSpectralProcessing
*/
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
