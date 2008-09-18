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

#ifndef LowLatencyConvolution_hxx
#define LowLatencyConvolution_hxx

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

class LowLatencyConvolution : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, unsigned, MaxFrames);
	    DYN_ATTRIBUTE( 1, public, int, FrameSize);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
		  SetMaxFrames(0);
	    };
	};

private:
	InPort<ComplexSpectrum> _input;
	OutPort<ComplexSpectrum> _output;
	InPort< std::vector<ComplexSpectrum>* > _impulseResponse;

	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;
	ComplexSpectrumProduct _product;
	ComplexSpectrumSum _sum;

public:
	const char* GetClassName() const { return "LowLatencyConvolution"; }
	LowLatencyConvolution(const Config& config = Config()) 
		: _input("Input", this)
		, _output("Output", this)
		, _impulseResponse("ImpulseResponse", this)
	{
		Configure( config );
	}
	unsigned GetLimitedFrames(unsigned requiredNFrames) const
	{
		unsigned maxFrames = _config.HasMaxFrames()? _config.GetMaxFrames() : 0;
		return maxFrames? std::min(maxFrames, requiredNFrames) : requiredNFrames;
	}
	void InitialitzeDelaySpectrums( unsigned nBlocks, unsigned spectrumSize, TData spectralRange)
	{
		std::cout << "LowLatencyConvolution: N blocks " << nBlocks << std::endl;
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
		const ComplexSpectrum & input = _input.GetData();
		ComplexSpectrum & output = _output.GetData();
		std::vector<ComplexSpectrum> & impulseResponse = *_impulseResponse.GetData();
		unsigned nBlocks = GetLimitedFrames(impulseResponse.size());
		if (_delayedSpectrums.size()!=nBlocks)
		{
			InitialitzeDelaySpectrums( nBlocks, impulseResponse[0].bins.size(), impulseResponse[0].spectralRange );
		}

		output.spectralRange = input.spectralRange;
		const unsigned nBins = input.bins.size(); 
		output.bins.resize( nBins );
		for (unsigned i=0; i<output.bins.size(); i++)
			output.bins[i]=0;

		_delayedSpectrums[_current].bins=input.bins;
		unsigned delayIndex=_current+1;
		ComplexSpectrum productSpectrum;
		for (unsigned i=0; i<nBlocks; i++)
		{
			if (delayIndex>=nBlocks) delayIndex=0;
			_product.Do(impulseResponse[nBlocks-i-1], _delayedSpectrums[delayIndex++], productSpectrum);
			_sum.Do(productSpectrum, output, output);
		}
		_current++;
		if (_current>=nBlocks) 
		{
			_current=0;
		}
		// Tell the ports this is done
		_input.Consume();
		_impulseResponse.Consume();
		_output.Produce();
		return true;
	}
};

} // namespace CLAM

#endif // LowLatencyConvolution_hxx
