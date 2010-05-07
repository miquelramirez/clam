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

#ifndef DOPPLERFRACTIONALDELAY_INCLUDED
#define DOPPLERFRACTIONALDELAY_INCLUDED

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
class DopplerFractionalDelay : public Processing
{
public:
	class Config : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config , 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, MaxDelayInSeconds);
		//DYN_ATTRIBUTE (1, public, float, SourcePosition);
		//DYN_ATTRIBUTE (2, public, float, ListenerPosition);
		DYN_ATTRIBUTE (1, public, float, Distance);

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelayInSeconds(1.3653125);  //65535
			//SetSourcePosition(0);
			//SetListenerPosition(25);
			SetDistance(0);
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
	
	Index _sampleRate, _delayBufferSize, _readIndex, _writeIndex, _modIndex;
	float _centerTap, _width, _freqMod;
	enum { C = 340, FPS=25};
	TData _pastModelayLine;
	float _pastDist, _step,_interpDist;
	bool _notInitialized;

protected:
	//FloatInControl _lPosition;
	//FloatInControl _sPosition;
	FloatInControl _distance;
		
	void setDelay(float delaySamples) 
	{
		CLAM_ASSERT(_delayBufferSize, "setDelay: Zero delay delay is not allowed!");
		Index delayInSamples = round(delaySamples);
		
		if (delayInSamples > _delayBufferSize) 
			return;
		
		Index readIndex = (_writeIndex - delayInSamples) % _delayBufferSize;
		if (_readIndex % _delayBufferSize == readIndex)
			return;
		
		_readIndex = readIndex;		
		
	}
	
	TData delayLine(TData x, float frac,Index D)	
	{	
		CLAM_ASSERT(_delayBufferSize, "delayLine: Zero delay buffer is not allowed!");
		Index writeindex = _writeIndex++ % _delayBufferSize;
		Index readindex = _readIndex++ % _delayBufferSize;	
		
		_delayBuffer[writeindex] = x;

		//TData y = frac*_delayBuffer[readindex2] + (1-frac)*_delayBuffer[readindex];	
		TData y;
		if (readindex==0) y = _delayBuffer.back() + (1-frac) * _delayBuffer[readindex]-(1-frac)*_pastModelayLine;				
		else y = _delayBuffer[readindex-1] + (1-frac) * _delayBuffer[readindex] - (1-frac)*_pastModelayLine;		

		

		return y;
	}

public:
	DopplerFractionalDelay(const Config& config = Config()) 
		: _in1("InputBuffer", this)
		, _out1("OutputBuffer", this)
		, _delayBufferSize(1)  	
		//, _lPosition("ListenerPosition in mts", this)
		//, _sPosition("SourcePosition in mts", this)
		,_distance("relative distance in mts", this)
	{
		Configure( config );
	}

	bool ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(_config, c);
		_sampleRate = BackendSampleRate();	
		
		_delayBuffer.resize(_config.GetMaxDelayInSeconds() * _sampleRate);
		_delayBufferSize = _delayBuffer.size(); 
		_writeIndex = (_delayBufferSize-1); 
		_readIndex = (_delayBufferSize-1);		
		std::fill(_delayBuffer.begin(), _delayBuffer.end(), 0.);

		//_lPosition.DoControl(_config.GetListenerPosition());
		//_sPosition.DoControl(_config.GetSourcePosition());
		//_lPosition.SetBounds(0,500);
		//_sPosition.SetBounds(0,500);
		_distance.DoControl(_config.GetDistance());
		_distance.SetBounds(0,500);
		_pastModelayLine=0;
		_pastDist=0;
		_step=0;
		_interpDist=0;
		_notInitialized=true;

		return true;
	}
	
	const char* GetClassName() const { return "DopplerFractionalDelay"; }
	
	virtual ~DopplerFractionalDelay() {}
		
	const ProcessingConfig & GetConfig() const { return _config; }
	
	bool Do()
	{
		//bool newS_ControlArrived = not _sPosition.HasBeenRead();
		//bool newL_ControlArrived = not _lPosition.HasBeenRead();
		bool newControlArrived = not _distance.HasBeenRead();
		//std::cout<< (newControlArrived ? "newcontrol" : "noNewControl") << std::endl;
			
		const float C=340.0;
		const float shiftGain=1000.0;		
		const CLAM::Audio& in = _in1.GetData();
		const TData* inpointer = in.GetBuffer().GetPtr();		
		unsigned size = in.GetSize();
		
		CLAM::Audio& out = _out1.GetData();
		out.SetSize(size);
		TData* outpointer = out.GetBuffer().GetPtr();	
	
		if (newControlArrived)// or newL_ControlArrived)
			{			
			//TControlData distance = _lPosition.GetLastValue()-_sPosition.GetLastValue();
			TControlData distance = _distance.GetLastValue();
			if (_notInitialized){ 
				_pastDist=distance;
				_interpDist=_pastDist;
				_notInitialized=false; 
			}
			if (abs(distance-_pastDist)>10){
				_step=0;
				_pastDist=distance;
				_interpDist=_pastDist;
			}
			else{			
				_step=(distance-_pastDist)*FPS/_sampleRate;
				//if (distance > 0) _step=-_step;		
				_pastDist=distance;
			}
		}
				
		//float step=(distance-_pastDist)/size;			
		//float interpDist=_pastDist;
		
		for (unsigned i = 0; i < size; ++i){			
			float delay= (shiftGain*sqrt((_interpDist)*(_interpDist))/C);
			float D=floor(delay);			
			float frac=delay-D;					
			setDelay(D);
			outpointer[i] = delayLine(inpointer[i],frac,D);
			_pastModelayLine=outpointer[i];	
			_interpDist=_interpDist+_step;
			//if (step==0) std::cout << delay << "*************************************** " <<std::endl;	
			//std::cout << delay << " " << _interpDist << " " << _step <<std::endl;
			//std::cout << _pastDist << " " << distance <<std::endl;				
		}
		
		_in1.Consume();
		_out1.Produce();
		return true;
	}
};

} // namespace CLAM

#endif 

