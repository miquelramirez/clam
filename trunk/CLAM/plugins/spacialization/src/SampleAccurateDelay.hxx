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

	unsigned _channels;
	
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
			double scaler = 1./(CROSSFADESIZE-i);
			for (unsigned channel=0; channel<_channels; channel++)
			{
				_crossFadeBuffer[channel][i] = _delayBuffer[channel][readIndex % _delayBufferSize] * scaler;
			}
			readIndex++;
		}
						
		return;
	}
	void delayLine(const CLAM::Audio & in, CLAM::Audio & out, unsigned channel) 
	{	
		unsigned inSize = in.GetSize();
		const TData* inpointer = in.GetBuffer().GetPtr();
		TData* outpointer = out.GetBuffer().GetPtr();

		Index writeIndexTemp(_writeIndex);
		Index readIndexTemp(_readIndex);
		Index crossFadeIndexTemp(_crossFadeIndex);

		for (unsigned i = 0; i < inSize; ++i)
		{
			Index writeindex = writeIndexTemp % _delayBufferSize;
			Index readindex = readIndexTemp % _delayBufferSize;

			_delayBuffer[channel][writeindex] = inpointer[i];
			outpointer[i] = _delayBuffer[channel][readindex];
			if (crossFadeIndexTemp > 0)
				outpointer[i] *= (1./(crossFadeIndexTemp)) + _crossFadeBuffer[channel][--crossFadeIndexTemp];
			writeIndexTemp++;
			readIndexTemp++;

			if (channel == _channels-1) // if is the last channel update the indexes
			{
				_writeIndex=writeIndexTemp;
				_readIndex=readIndexTemp;
				_crossFadeIndex=crossFadeIndexTemp;
			}

		}

	}
public:
	SampleAccurateDelay(const Config& config = Config()) 
		: _delayBufferSize(1)
		, _crossFadeIndex(0)
		, _channels(1)
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

