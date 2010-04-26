/*
 * Copyright (c) 2010 Fundació Barcelona Media Universitat Pompeu Fabra
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

#ifndef FRACTIONALDELAYMODULATION_INCLUDED
#define FRACTIONALDELAYMODULATION_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
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
class FractionalDelayModulation : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 4, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, MaxDelayInSeconds);
		DYN_ATTRIBUTE (1, public, float, Width);
		DYN_ATTRIBUTE (2, public, float, FreqMod);
		DYN_ATTRIBUTE (3, public, float, CenterTap);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelayInSeconds(1.3653125);  //65535
			SetWidth(1);
			SetFreqMod(5);
			SetCenterTap(2);
		}
	};
	
//private: //TODO debugging	
protected:
	Config _config;
	typedef long long unsigned Index;
	typedef std::vector<TData> DelayBuffer;
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	OutPort<Audio> _out2;
	DelayBuffer _delayBuffer1;
	DelayBuffer _delayBuffer2;
	Index _sampleRate, _delayBufferSize, _readIndex1, _writeIndex1, _modIndex;
	Index _readIndex2, _writeIndex2;
	enum { CROSSFADESIZE = 150};
	TData _pastModelayLine1,_pastModelayLine2;
		

	DelayBuffer _crossFadeBuffer;

protected:
	FloatInControl _width;
	FloatInControl _freqMod;
	FloatInControl _centerTap;
	
	void setDelay1(float delaySamples) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex1 = (_writeIndex1 - delayInSamples) % _delayBufferSize ;

		if (_readIndex1 % _delayBufferSize == readIndex1)
			return;
		
		_readIndex1 = readIndex1;		
		/*_crossFadeIndex = CROSSFADESIZE;
		
		for (Index i = 0; i < CROSSFADESIZE; i++) 
			_crossFadeBuffer[i] = _delayBuffer[readIndex++ % _delayBufferSize] * (1./(CROSSFADESIZE-i));*/				
		return;
	}

	void setDelay2(float delaySamples) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex2 = (_writeIndex2 - delayInSamples) % _delayBufferSize ;

		if (_readIndex2 % _delayBufferSize == readIndex2)
			return;
		
		_readIndex2 = readIndex2;		
		/*_crossFadeIndex = CROSSFADESIZE;
		
		for (Index i = 0; i < CROSSFADESIZE; i++) 
			_crossFadeBuffer[i] = _delayBuffer[readIndex++ % _delayBufferSize] * (1./(CROSSFADESIZE-i));*/				
		return;
	}
	
	TData delayLine1(TData x1, float frac1)
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");
		Index writeindex1 = _writeIndex1++ % _delayBufferSize;
		Index readindex1 = _readIndex1++ % _delayBufferSize;			
		
		_delayBuffer1[writeindex1] = x1;
		
		TData y1 = _delayBuffer1[readindex1-1] + (1-frac1)*_delayBuffer1[readindex1]- (1-frac1)*_pastModelayLine1;
				
		
		//if (_crossFadeIndex > 0)  y *= (1./_crossFadeIndex) + _crossFadeBuffer[--_crossFadeIndex];
						
		return y1;
	}

	TData delayLine2(TData x2, float frac2)
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");
		Index writeindex2 = _writeIndex2++ % _delayBufferSize;
		Index readindex2 = _readIndex2++ % _delayBufferSize;		
				
		_delayBuffer2[writeindex2] = x2;
		//TData y2 = frac2*_delayBuffer2[readindex2-1] + (1-frac2)*_delayBuffer2[readindex2];	
		TData y2 = _delayBuffer2[readindex2-1] + (1-frac2)*_delayBuffer2[readindex2]- (1-frac2)*_pastModelayLine2;

		
            		
						
		return y2;
	}


public:
	FractionalDelayModulation(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _out1("OutputBufferL", this)
		, _out2("OutputBufferR", this)
		, _delayBufferSize(1)  
		, _width("width in msecs", this)
		, _freqMod("freq in Hz", this)
		, _centerTap("centerTap msecs", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		_sampleRate = BackendSampleRate();
		
		_crossFadeBuffer.resize(CROSSFADESIZE);
		std::fill(_crossFadeBuffer.begin(), _crossFadeBuffer.end(), 0.);
		
		_delayBuffer1.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBuffer2.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBufferSize = _delayBuffer1.size(); 
		_readIndex1 = _writeIndex1 = (_delayBufferSize-1); 
		_readIndex2 = _writeIndex2 = (_delayBufferSize-1);
		std::fill(_delayBuffer1.begin(), _delayBuffer1.end(), 0.);
		std::fill(_delayBuffer2.begin(), _delayBuffer2.end(), 0.);
		
		_pastModelayLine1=0;
		_pastModelayLine2=0;
		_modIndex=0;		
		_width.DoControl(0);
		_freqMod.DoControl(0.);
		_centerTap.DoControl(0.);

		return true;
	}
	

	const char* GetClassName() const { return "FractionalDelayModulation"; }
	
	virtual ~FractionalDelayModulation() {}
		
	const ProcessingConfig & GetConfig() const { return _config; }
	
	bool Do()
	{
		const float B=0.7071;
		const float FB=-0.7071;	
		const float FF=0.7071;

		TControlData width = _width.GetLastValue();
		TControlData freqMod = _freqMod.GetLastValue();
		TControlData centerTap = _centerTap.GetLastValue();
		freqMod=freqMod/_sampleRate;
		width=round(width/1000*_sampleRate);
		centerTap=round(centerTap/1000*_sampleRate);
						
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out1 = _out1.GetData();
		out1.SetSize(size);
		TData* outpointer1 = out1.GetBuffer().GetPtr();	

		CLAM::Audio& out2 = _out2.GetData();
		out2.SetSize(size);
		TData* outpointer2 = out2.GetBuffer().GetPtr();	

		for (unsigned i = 0; i < size; ++i){
			//TControlData delay = _delayControl.GetLastValue();
			
			float delay1=1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex));
			float delay2=1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex));  //change to cos when using chorus
			float D1=floor(delay1);
			float D2=floor(delay2);			
			float frac1=delay1-D1;
			float frac2=delay2-D2;			
			setDelay1(centerTap);
			setDelay2(centerTap);
			TData FixdelayLine1=_delayBuffer1[_readIndex1];
			TData FixdelayLine2=_delayBuffer2[_readIndex2];
			TData x1=inpointer[i]-FB*FixdelayLine1;
			TData x2=inpointer[i]-FB*FixdelayLine2;
			setDelay1(D1);
			setDelay2(D2);
			TData ModelayLine1 = delayLine1(x1,frac1);
			TData ModelayLine2 = delayLine2(x2,frac2);
			outpointer1[i] = FF*ModelayLine1 + B*x1;
			outpointer2[i] = FF*ModelayLine2 + B*x2;
			_pastModelayLine1=ModelayLine1;
        		_pastModelayLine2=ModelayLine2;
			_modIndex++;
		}
		
		_in1.Consume();
		_out1.Produce();
		_out2.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

