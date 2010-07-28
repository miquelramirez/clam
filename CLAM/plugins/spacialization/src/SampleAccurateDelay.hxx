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

#ifndef SAMPLEACCURATEDELAY_INCLUDED
#define SAMPLEACCURATEDELAY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/InControl.hxx>

#include <vector>
#include <cmath>

namespace CLAM
{

/**
 *
 */
class SampleAccurateDelay : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 4, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, MaxDelayInSeconds);
		DYN_ATTRIBUTE (1, public, unsigned, SampleRate);
		DYN_ATTRIBUTE (2, public, unsigned, InitialDelayInSamples);
		DYN_ATTRIBUTE( 3, public, int, PortsNumber);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelayInSeconds(1.3653125);  //65535
			SetSampleRate(48000);
			SetInitialDelayInSamples(0);
			SetPortsNumber(1);
		}
	};
	
//private: //TODO debugging	
protected:
	Config _config;
	typedef long long unsigned Index;
	typedef std::vector<TData> DelayBuffer;
	std::vector<DelayBuffer> _delayBuffer;
	
	Index _sampleRate, _delayBufferSize, _readIndex, _writeIndex, _crossFadeIndex;
	
	enum { CROSSFADESIZE = 150 };
	std::vector<DelayBuffer> _crossFadeBuffer;

protected:
	FloatInControl _delayControl;
	
	void setDelay(float delaySamples) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex = (_writeIndex - delayInSamples) % _delayBufferSize ;

		if (_readIndex % _delayBufferSize == readIndex)
			return;
		
		_readIndex = readIndex;		
		_crossFadeIndex = CROSSFADESIZE;
		
		for (Index i = 0; i < CROSSFADESIZE; i++) 
		{
			for (unsigned channel=0; channel<_delayBuffer.size(); channel++)
			{
				_crossFadeBuffer[channel][i] = _delayBuffer[channel][readIndex % _delayBufferSize] * (1./(CROSSFADESIZE-i));
			}
			readIndex++;
		}
						
		return;
	}
	
	TData delayLine(TData x, unsigned channel=0) //TODO: remove default val when BufferDelay updated
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");
		Index writeindex = _writeIndex % _delayBufferSize;
		Index readindex = _readIndex % _delayBufferSize;

		_delayBuffer[channel][writeindex] = x;
		TData y = _delayBuffer[channel][readindex];
		if (_crossFadeIndex > 0)
			y *= (1./(_crossFadeIndex)) + _crossFadeBuffer[channel][_crossFadeIndex-1];
		
		return y;
	}
		
public:
	SampleAccurateDelay(const Config& config = Config()) 
		: _delayBufferSize(1)
		, _crossFadeIndex(0)
		, _delayControl("Delay in Samples", this)
	{
		Configure( config );
	}
	
	virtual ~SampleAccurateDelay() {}
	
	const char* GetClassName() const { return "SampleAccurateDelay"; }
		
	
	const ProcessingConfig & GetConfig() const { return _config; }
	
};

} // namespace CLAM

#endif 

