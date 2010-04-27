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

#ifndef FLANGER_INCLUDED
#define FLANGER_INCLUDED

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
class  Flanger: public FractionalDelayModulation
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
			SetWidth(2);
			SetFreqMod(7);
			SetCenterTap(5);
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
		delay.push_back(1+(0.8*centerTap)+(width*sin(2*M_PI*freqMod*_modIndex))); 
		_modIndex++;
		return delay;
	}

	virtual void _initSpecificParameters(){
		_width.SetBounds(0,3);
		_freqMod.SetBounds(0,10);
		_centerTap.SetBounds(0,15);
		_blend=0.7071;
		_feedback=-0.7071;
		_feedForward=0.7071;
		
	}
public:
	const char* GetClassName() const { return "Flanger"; }
	virtual ~Flanger() {}
};

} // namespace CLAM

#endif 

