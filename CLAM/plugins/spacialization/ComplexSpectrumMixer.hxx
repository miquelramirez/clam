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

#ifndef ComplexSpectrumMixer_hxx
#define ComplexSpectrumMixer_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

class ComplexSpectrumMixer : public Processing
{ 
	InPort<ComplexSpectrum> _term1;
	InPort<ComplexSpectrum> _term2;
	OutPort<ComplexSpectrum> _sum;
	InControl _gain1;
	InControl _gain2;
public:
	const char* GetClassName() const { return "ComplexSpectrumMixer"; }
	ComplexSpectrumMixer(const Config& config = Config()) 
		: _term1("Term1", this)
		, _term2("Term2", this)
		, _sum("Mixer", this) 
		, _gain1("Gain1", this)
		, _gain2("Gain2", this)
	{
		Configure( config );
		_gain1.SetBounds(0,1);
		_gain2.SetBounds(0,1);
	}

	bool Do(const ComplexSpectrum & in1, const ComplexSpectrum & in2, ComplexSpectrum & out)
	{
		TControlData gain1 = _gain1.GetLastValue();
		TControlData gain2 = _gain2.GetLastValue();
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
			out.bins[i] = (gain1*in1.bins[i]) + (gain2*in2.bins[i]);
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

#endif // ComplexSpectrumMixer_hxx
