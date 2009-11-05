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

#ifndef SAMPLEACCURATEBUFFERDELAY_INCLUDED
#define SAMPLEACCURATEBUFFERDELAY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/InControl.hxx>

#include <vector>

namespace CLAM
{

class SampleAccurateBufferDelayConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SampleAccurateBufferDelayConfig , 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, MaxDelayInSamples);
	DYN_ATTRIBUTE (1, public, int, SampleRate);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetMaxDelayInSamples(65536);
		SetSampleRate(48000);
	}
};


/**
 *
 */
class SampleAccurateBufferDelay : public Processing
{
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	InControl _delayControl;
	
	SampleAccurateBufferDelayConfig _config;
	
	typedef std::vector<TData> DelayBuffer;
	DelayBuffer _delayBuffer;
	
	unsigned _sampleRate, _delayBufferSize, _delayInSamples, 
			_readIndex, _writeIndex, _crossFadeIndex;
	
	enum { CROSSFADESIZE = 100 };
	DelayBuffer _crossFadeBuffer;

	void setDelay(float delayInSec) 
	{
		unsigned delayInSamples = delayInSec * _sampleRate;
		
		if (_delayInSamples > _delayBufferSize) 
			return;
		
		unsigned readIndex = (_writeIndex - delayInSamples) % _delayBufferSize ;
		
		if (readIndex == _readIndex)
			return;
		
		_readIndex = readIndex;
		_crossFadeIndex = CROSSFADESIZE;
		
		for (unsigned i = 0; i < CROSSFADESIZE; i++) 
			_crossFadeBuffer[i]=_delayBuffer[readIndex++%_delayBufferSize] * (1./(CROSSFADESIZE-i));
						
		return;
	}
	
	TData delayLine(TData x)
	{	
		_delayBuffer[_writeIndex++%_delayBufferSize] = x;
		TData y = _delayBuffer[_readIndex++%_delayBufferSize];
		
		if (_crossFadeIndex > 0)  
			y *= (1./_crossFadeIndex) + _crossFadeBuffer[--_crossFadeIndex];
				
		return y;
	}
		
public:
	const char* GetClassName() const { return "SampleAccurateBufferDelay"; }
	SampleAccurateBufferDelay() 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _delayControl("VaryingDelay", this)
		, _delayInSamples(0.)
		, _crossFadeIndex(0)
		
	{
		Configure( _config );
		_sampleRate = _config.GetSampleRate();
		
		_crossFadeBuffer.resize(CROSSFADESIZE);
		std::fill(_crossFadeBuffer.begin(), _crossFadeBuffer.end(), 0.);
		
		_delayBuffer.resize(_config.GetMaxDelayInSamples());
		_delayBufferSize = _delayBuffer.size(); 
		_readIndex = _writeIndex = (_delayBufferSize-1); 
		
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);
	}
	
	bool Do()
	{
		TControlData delay = _delayControl.GetLastValue();
		setDelay(delay);
		
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetData();
		out.SetSize(size);
		TData* outpointer = out.GetBuffer().GetPtr();
		
		for (unsigned i = 0; i < size; ++i) 
			outpointer[i] = delayLine(inpointer[i]);
		
		// Tell the ports this is done
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

