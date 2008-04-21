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

#ifndef DebugImpulseResponse_hxx
#define DebugImpulseResponse_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"
#include "ComplexSpectrum.hxx"
#include "ComplexSpectrumProduct.hxx"
#include "ComplexSpectrumSum.hxx"
#include "ComplexSpectrumMixer.hxx"
#include <algorithm>
#include <cmath>
#include <algorithm>

namespace CLAM
{

class DebugImpulseResponse : public Processing
{ 

private:
	OutPort<ComplexSpectrum> _output;
	InPort< std::vector<ComplexSpectrum>* > _impulseResponse;

	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;
	ComplexSpectrumProduct _product;
	ComplexSpectrumSum _sum;

public:
	const char* GetClassName() const { return "DebugImpulseResponse"; }
	DebugImpulseResponse(const Config& config = Config()) 
		: _output("Output", this)
		, _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	void InitialitzeDelaySpectrums( unsigned nBlocks, unsigned spectrumSize, TData spectralRange)
	{
		std::cout << "DebugImpulseResponse: N blocks " << nBlocks << std::endl; 
		_delayedSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _delayedSpectrums[i];
			spectrum.spectralRange=spectralRange;
			spectrum.bins.assign( spectrumSize, std::complex<CLAM::TData>() );
		}
		_current=0;
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }


	bool Do()
	{
		ComplexSpectrum & output = _output.GetData();
		std::vector<ComplexSpectrum> & impulseResponse = *_impulseResponse.GetData();
		unsigned nBlocks = std::min(impulseResponse.size(), size_t(1000));

		output.spectralRange = impulseResponse[0].spectralRange;
		output.bins = impulseResponse[0].bins;

		unsigned delayIndex=_current+1;
		// Tell the ports this is done
		_impulseResponse.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // DebugImpulseResponse_hxx
