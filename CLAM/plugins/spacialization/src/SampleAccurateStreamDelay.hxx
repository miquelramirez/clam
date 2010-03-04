/*
 * Copyright (c) 2009 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef SAMPLEACCURATESTREAMDELAY_INCLUDED
#define SAMPLEACCURATESTREAMDELAY_INCLUDED

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include "SampleAccurateDelay.hxx"

namespace CLAM
{

/**
 *
 */
class SampleAccurateStreamDelay : public SampleAccurateDelay
{
	AudioInPort _in;
	AudioOutPort _out;

public:
	SampleAccurateStreamDelay(const Config& config = Config()) 
		: _in("InputStream", this)
		, _out("OutputStream", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);	
		_sampleRate = _config.GetSampleRate();
		
		_crossFadeBuffer.resize(CROSSFADESIZE);
		std::fill(_crossFadeBuffer.begin(), _crossFadeBuffer.end(), 0.);
		
		_delayBuffer.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBufferSize = _delayBuffer.size(); 
		_readIndex = _writeIndex = (_delayBufferSize-1); 
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);
		_delayControl.DoControl(0.);
		const unsigned buffersize = BackendBufferSize();
		_in.SetSize(buffersize);
		_in.SetHop(buffersize);
		_out.SetSize(buffersize);
		_out.SetHop(buffersize);

		
		return true;
	}
	
	const char* GetClassName() const { return "SampleAccurateStreamDelay"; }
	
	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);
		
		const CLAM::Audio& in = _in.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		CLAM::Audio& out = _out.GetAudio();
		TData* outpointer = out.GetBuffer().GetPtr();

		unsigned insize = in.GetSize();
		
		for (unsigned i = 0; i < insize; ++i) 
			outpointer[i] = delayLine(inpointer[i]);
		
		_in.Consume();
		_out.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

