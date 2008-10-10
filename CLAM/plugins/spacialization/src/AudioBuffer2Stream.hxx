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

#ifndef AudioBuffer2Stream_hxx
#define AudioBuffer2Stream_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <algorithm>

namespace CLAM
{

/**
 Processing that take buffers of audio and produces an audio
 stream by concatenating or semioverlaping them.
 @param Hopsize [Config] How many samples the window is advancing each execution 
 @param FFTSize [Config] How many samples the window includes
 @param[in] "Audio buffer"  [Port]
 @ingroup NewSpectralProcessing New spectral processing
*/

class AudioBuffer2Stream : public Processing
{
	// TODO: Use that configuration instead AudioWindowingConfig
	class FutureConfig : public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( FutureConfig, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, HopSize);
		// TODO: Rename FFTSize to WindowSize
		DYN_ATTRIBUTE( 1, public, unsigned, FFTSize);
    	protected:
	    void DefaultInit()
	    {
		AddAll();
		UpdateData();
		SetHopSize(512);
		SetFFTSize(1024);
	    }
	};
	typedef AudioWindowingConfig Config;
	InPort<Audio> _in;
	AudioOutPort _out;
	unsigned _hopSize;
	unsigned _windowSize;
	Config _config;
public:
	const char* GetClassName() const { return "AudioBuffer2Stream"; }
	AudioBuffer2Stream(const Config& config = Config()) 
		: _in("Audio buffer", this)
		, _out("Audio stream", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(_config, c);
		_hopSize = _config.GetHopSize();
		_windowSize = _config.GetFFTSize();
		_out.SetSize( _windowSize );
		_out.SetHop( _hopSize );
		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }
 
	bool Do()
	{
		const Audio& in = _in.GetData();
		CLAM_ASSERT(_windowSize==in.GetSize(),
			"AudioBuffer2Stream: Input does not provide the configured window size"); 
		Audio& out = _out.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();
		TData* outpointer = out.GetBuffer().GetPtr();
		// Zero fill the new incomming hopSize
		std::fill(outpointer+_windowSize-_hopSize, outpointer+_windowSize, 0.0);
		// Add the input on the full window
		std::transform(inpointer, inpointer+_windowSize, outpointer, outpointer, std::plus<TData>());
		
		// Tell the ports this is done
		_in.Consume();
		_out.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // AudioBuffer2Stream_hxx
