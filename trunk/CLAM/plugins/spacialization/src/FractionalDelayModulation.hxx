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
	DelayBuffer _delayBuffers[2];
	Index _readIndexes[2];
	Index _writeIndexes[2];
	Index _sampleRate, _delayBufferSize, _modIndex;
	enum { CROSSFADESIZE = 150};	
	TData _pastModelayLine[2];
		

	DelayBuffer _crossFadeBuffer;

protected:
	FloatInControl _width;
	FloatInControl _freqMod;
	FloatInControl _centerTap;
	

	void setDelay(float delaySamples,unsigned i) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex = (_writeIndexes[i] - delayInSamples) % _delayBufferSize ;

		if (_readIndexes[i] % _delayBufferSize == readIndex)
			return;
		
		_readIndexes[i] = readIndex;					
		return;
	}

	TData delayLine(TData x, float frac,unsigned i)
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");		
		Index writeindex = _writeIndexes[i]++ % _delayBufferSize;
		Index readindex = _readIndexes[i]++ % _delayBufferSize;		
				
		_delayBuffers[i][writeindex] = x;
		//TData y2 = frac2*_delayBuffer2[readindex2-1] + (1-frac2)*_delayBuffer2[readindex2];
		TData y = _delayBuffers[i][readindex-1] + (1-frac) * _delayBuffers[i][readindex] - (1-frac)*_pastModelayLine[i];
								
		return y;
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
		
		for (unsigned i=0;i<2;i++){
			_delayBuffers[i].resize(_config.GetMaxDelayInSeconds() * _sampleRate);
			_delayBufferSize = _delayBuffers[i].size();
			_readIndexes[i] = _writeIndexes[i] = (_delayBufferSize-1); 
			std::fill(_delayBuffers[i].begin(), _delayBuffers[i].end(), 0.);
			_pastModelayLine[i]=0;
		}		
	
		_modIndex=0;		

		_width.DoControl(_config.GetWidth());
		_freqMod.DoControl(_config.GetFreqMod());
		_centerTap.DoControl(_config.GetCenterTap());

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

		for (unsigned j = 0; j < size; ++j){
			float delay[2];
			TData FixDelayLine[2];
			TData ModelayLine[2];
			TData x[2];
			
			delay[0]=1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex));
			delay[1]=1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex));  //change to cos when using chorus
			
			for (unsigned i=0;i<2;i++){
				float D=floor(delay[i]);
				float frac=delay[i]-D;
				setDelay(centerTap,i);
				FixDelayLine[i]=_delayBuffers[i][_readIndexes[i]];
				x[i]=inpointer[j]-FB*FixDelayLine[i];
				setDelay(D,i);
				ModelayLine[i] = delayLine(x[i],frac,i);
				_pastModelayLine[i]=ModelayLine[i];
			}
			outpointer1[j] = FF*ModelayLine[0] + B*x[0];
			outpointer2[j] = FF*ModelayLine[1] + B*x[1];
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

