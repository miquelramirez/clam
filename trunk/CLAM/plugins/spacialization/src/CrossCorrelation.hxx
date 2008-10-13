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

#ifndef CrossCorrelation_hxx
#define CrossCorrelation_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include "ComplexSpectrum.hxx"
#include <algorithm>

namespace CLAM
{

/**
 Processing that computes the time domain cross correlation of two
 spectrums (which is cheaper performed in spectral domain).
 @param[in] Input1 [Port] A ComplexSpectrum.
 @param[in] Input2 [Port] A ComplexSpectrum.
 @param[out] CrossCorrelation [Port] A ComplexSpectrum whose time domain representation is the crosscorrelation of the two inputs timedomain representations
 @pre Input1 and Input2 must be equal size.
 @pre Input1 and Input2 must have the same spectralRange.
 @ingroup SpectralProcessingOperations
*/
class CrossCorrelation : public Processing
{ 
	InPort<ComplexSpectrum> _factor1;
	InPort<ComplexSpectrum> _factor2;
	OutPort<ComplexSpectrum> _product;
public:
	const char* GetClassName() const { return "CrossCorrelation"; }
	CrossCorrelation(const Config& config = Config()) 
		: _factor1("Input1", this)
		, _factor2("Input2", this)
		, _product("CrossCorrelation", this) 
	{
		Configure( config );
	}

	bool Do(const ComplexSpectrum & in1, const ComplexSpectrum & in2, ComplexSpectrum & out)
	{
		if ( in1.bins.size()!=in2.bins.size())
		{
			std::ostringstream os;
			os
				<< "Factors should be of equal size:"
				<< " Factor 1: " << in1.bins.size()
				<< " Factor 2: " << in2.bins.size()
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}
		if (in1.spectralRange!=in2.spectralRange)
		{
			std::ostringstream os;
			os 
				<< "SpectralRanges should be equal:"
				<< " Range 1: " << in1.spectralRange
				<< " Range 2: " << in2.spectralRange
				<< std::flush;
			CLAM_ASSERT(false, os.str().c_str());
		}

		out.spectralRange = in1.spectralRange;
		const unsigned nBins = in1.bins.size(); 
		if (out.bins.size()!=nBins) // Also protects against resizing when out is one of the ins
			out.bins.resize( nBins );
		std::complex<float> * outBins = & out.bins[0];
		const std::complex<float> * in1Bins = & in1.bins[0];
		const std::complex<float> * in2Bins = & in2.bins[0];
		for (unsigned i=0; i<nBins; i++)
		{
			outBins[i] = std::conj(in1Bins[i]) * in2Bins[i];
		}
		return true;
	}
	bool Do()
	{
		const ComplexSpectrum & factor1 = _factor1.GetData();
		const ComplexSpectrum & factor2 = _factor2.GetData();
		ComplexSpectrum & product = _product.GetData();

		Do(factor1, factor2, product);

		// Tell the ports this is done
		_factor1.Consume();
		_factor2.Consume();
		_product.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // CrossCorrelation_hxx
