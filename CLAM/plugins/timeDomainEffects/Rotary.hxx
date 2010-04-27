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

#ifndef ROTARY_INCLUDED
#define ROTARY_INCLUDED

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>
#include <CLAM/InControl.hxx>
#include "FractionalDelayModulation.hxx"

#include <vector>
#include <cmath>

namespace CLAM
{

/**
 *
 */
class  Rotary: public FractionalDelayModulation
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
			SetWidth(0.5);
			SetFreqMod(8);
			SetCenterTap(10);
		}
	};
	
//private: //TODO debugging	
	static const unsigned _nChannels=2;
protected:
	Config _config;
	virtual std::vector<float> getFractionalDelayValue (TControlData & width,TControlData & freqMod, TControlData & centerTap) 
	{
		std::vector<float> delay;
		delay.push_back(1+centerTap+(width*sin(2*M_PI*freqMod*_modIndex)));
		delay.push_back(1+(0.1*centerTap)+(width*cos(2*M_PI*freqMod*_modIndex))); 		
		return delay;
	}

	virtual void _initSpecificParameters(){
		_width.SetBounds(0,2);
		_freqMod.SetBounds(0,10);
		_centerTap.SetBounds(0,15);
		_blend=0;
		_feedback=0;
		_feedForward=1;
		
	}
public:
	const char* GetClassName() const { return "Rotary"; }
	virtual ~Rotary() {}

	virtual bool Do()
	{

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
			std::vector<float> delay=getFractionalDelayValue(width,freqMod,centerTap);
			TData FixDelayLine[_nChannels];
			TData ModelayLine[_nChannels];
			TData x[_nChannels];
			
			for (unsigned i=0;i<_nChannels;i++){
				float D=floor(delay[i]);
				float frac=delay[i]-D;
				setDelay(centerTap,i);
				FixDelayLine[i]=_delayBuffers[i][_readIndexes[i]];
				x[i]=inpointer[j]-_feedback*FixDelayLine[i];
				setDelay(D,i);
				ModelayLine[i] = delayLine(x[i],frac,i);
				_pastModelayLine[i]=ModelayLine[i];
			}
			TData outL =  (1+sin(2*M_PI*freqMod*_modIndex)) * (_feedForward*ModelayLine[0] + _blend*x[0]);
			TData outR =  (1+cos(2*M_PI*freqMod*_modIndex)) * (_feedForward*ModelayLine[1] + _blend*x[1]);

			outpointer1[j] = outL + 0.7* outR;
			outpointer2[j] = outR + 0.7* outL;			

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

