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

#ifndef MONOFRACTIONALDELAYMODULATION_INCLUDED
#define MONOFRACTIONALDELAYMODULATION_INCLUDED

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
class MonoFractionalDelayModulation : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, MaxDelayInSeconds);
		DYN_ATTRIBUTE (1, public, float, Width);
		DYN_ATTRIBUTE (2, public, float, FreqMod);
		DYN_ATTRIBUTE (3, public, float, CenterTap);
		DYN_ATTRIBUTE (4, public, float, PhaseOffset);
		DYN_ATTRIBUTE (5, public, float, ParamVariability);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelayInSeconds(1.3653125);  //65535
			SetWidth(1);
			SetFreqMod(5);
			SetCenterTap(2);
			SetPhaseOffset(0);
			SetParamVariability(1);
		}
	};
	
//private: //TODO debugging	
	
protected:
	Config _config;
	typedef long long unsigned Index;
	typedef std::vector<TData> DelayBuffer;
	InPort<Audio> _in1;
	OutPort<Audio> _out1;
	DelayBuffer _delayBuffer;
	Index _readIndex, _writeIndex, _sampleRate, _delayBufferSize, _modIndex;	
	TData _pastModelayLine;


protected:
	FloatInControl _width;
	FloatInControl _freqMod;
	FloatInControl _centerTap;
	FloatInControl _phaseOffset;
	FloatInControl _paramVariability;
	float _blend;
	float _feedback;	
	float _feedForward;
	
	virtual float getFractionalDelayValue (TControlData & width,TControlData & freqMod, TControlData & centerTap) 
	{
		float offset=_phaseOffset.GetLastValue()*M_PI/180;
		float delay=1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex + offset));
		_modIndex++;
		return delay;
	}

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
		return;
	}

	TData delayLine(TData x, float frac)
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");		
		Index writeindex = _writeIndex++ % _delayBufferSize;
		Index readindex = _readIndex++ % _delayBufferSize;		
				
		_delayBuffer[writeindex] = x;
		
		TData y;
		if (readindex==0) y =_delayBuffer.back()+(1-frac)*_delayBuffer[readindex]-(1-frac)*_pastModelayLine;				
		else y = _delayBuffer[readindex-1] + (1-frac) * _delayBuffer[readindex] - (1-frac)*_pastModelayLine;

						
		return y;
	}

	virtual void _initSpecificParameters(){
		_width.SetBounds(0,30);
		_freqMod.SetBounds(0,10);
		_centerTap.SetBounds(0,30);
		_phaseOffset.SetBounds(0,360);
		_paramVariability.SetBounds(0.5,1.5);
		_blend=0.7071;
		_feedback=0.7071;
		_feedForward=1;
		
	}


public:
	MonoFractionalDelayModulation(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _delayBufferSize(1)  
		, _width("width in msecs", this)
		, _freqMod("freq in Hz", this)
		, _centerTap("centerTap msecs", this)
		, _phaseOffset("offset in degrees", this)
		, _paramVariability("variability of parameters",this)
		, _blend(0)
		, _feedback(0)
		, _feedForward(0)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		_sampleRate = BackendSampleRate();
		
		
		_delayBuffer.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBufferSize = _delayBuffer.size();
		_readIndex = _writeIndex = (_delayBufferSize-1); 
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);
		_pastModelayLine=0;
				
	
		_modIndex=0;		

		_width.DoControl(_config.GetWidth());
		_freqMod.DoControl(_config.GetFreqMod());
		_centerTap.DoControl(_config.GetCenterTap());
		_phaseOffset.DoControl(_config.GetPhaseOffset());
		//_paramVariability.DoControl(_config.GetParamVariability());

		_initSpecificParameters();

		return true;
	}
	

	const char* GetClassName() const { return "MonoFractionalDelayModulation"; }
	
	virtual ~MonoFractionalDelayModulation() {}
		
	const ProcessingConfig & GetConfig() const { return _config; }
	
	virtual bool Do()
	{

		TControlData width = _width.GetLastValue();
		TControlData freqMod = _freqMod.GetLastValue();
		TControlData centerTap = _centerTap.GetLastValue();
		TControlData factor = _paramVariability.GetLastValue();		
		freqMod=(freqMod/_sampleRate)*factor;
		width=round(width/1000*_sampleRate)*factor;
		centerTap=round(centerTap/1000*_sampleRate)*factor;
						
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out1 = _out1.GetData();
		out1.SetSize(size);
		TData* outpointer1 = out1.GetBuffer().GetPtr();	

		for (unsigned j = 0; j < size; ++j){
			float delay=getFractionalDelayValue(width,freqMod,centerTap);			
			float D=floor(delay);
			float frac=delay-D;
			setDelay(centerTap);
			TData FixDelayLine=_delayBuffer[_readIndex];
			TData x=inpointer[j]-_feedback*FixDelayLine;
			setDelay(D);
			TData ModelayLine = delayLine(x,frac);
			_pastModelayLine=ModelayLine;
			outpointer1[j] = _feedForward*ModelayLine + _blend*x;						
		}
		
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

