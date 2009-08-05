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

#ifndef ComplexConjugate_hxx
#define ComplexConjugate_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

/**
 Processing that conjugates the complex spectrum.
 @param[in] Input A ComplexSpectrum to be conjugated.
 @param[out] Output An ComplexSpectrum which is the same in the input but with all the bins conjugated.
 @ingroup SpectralProcessingOperations
*/
class ComplexConjugate : public Processing
{ 
	InPort<ComplexSpectrum> _input;
	OutPort<ComplexSpectrum> _output;
public:
	const char* GetClassName() const { return "ComplexConjugate"; }
	ComplexConjugate(const Config& config = Config()) 
		: _input("Input", this)
		, _output("Output", this) 
	{
		Configure( config );
	}

	bool Do(const ComplexSpectrum & in, ComplexSpectrum & out)
	{
		out.spectralRange = in.spectralRange;
		const unsigned nBins = in.bins.size(); 
		if (out.bins.size()!=nBins) // Also protects against resizing when out is one of the ins
			out.bins.resize( nBins );
		for (unsigned i=0; i<nBins; i++)
		{
			out.bins[i] = std::conj(in.bins[i]);
		}
		return true;
	}
	bool Do()
	{
		const ComplexSpectrum & input = _input.GetData();
		ComplexSpectrum & output = _output.GetData();

		Do(input, output);

		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // ComplexSpectrumInverter_hxx
