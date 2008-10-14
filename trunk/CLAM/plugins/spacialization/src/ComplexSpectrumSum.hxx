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

#ifndef ComplexSpectrumSum_hxx
#define ComplexSpectrumSum_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

/**
 Processing that adds two incoming complex spectrums.
 @param[in] Term1 [Port] A ComplexSpectrum to be added.
 @param[in] Term2 [Port] A ComplexSpectrum to be added.
 @param[out] Mixer [Port] A ComplexSpectrum which is the sum of the two inputs.
 @pre Term1 and Term2 must be equal size.
 @pre Term1 and Term2 must have the same spectralRange.
 @ingroup SpectralProcessingOperations
*/
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
		if (out.bins.size()!=nBins) // Also protects against resizing when out is one of the ins
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
