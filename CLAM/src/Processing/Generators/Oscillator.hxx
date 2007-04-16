/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
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

#ifndef __Oscillator_hxx__
#define __Oscillator_hxx__ 

#include "SimpleOscillator.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Audio.hxx"
#include "AudioInPort.hxx"
#include "InControl.hxx"
#include <string>

namespace CLAM
{
	
class OscillatorConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OscillatorConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData, Amplitude);
	DYN_ATTRIBUTE (2, public, TData, ModIndex);
	DYN_ATTRIBUTE (3, public, TData , Phase);
	DYN_ATTRIBUTE (4, public, TData , SamplingRate);
protected:
	void DefaultInit(void);
};

class Oscillator : public SimpleOscillator
{
	OscillatorConfig mConfig;

	AudioInPort mInputPhaseMod;
	AudioInPort mInputFreqMod;
	TData mModIndex;

	typedef InControlTmpl<Oscillator> OscillatorCtrl;

	bool mModIdxUpdated;
	Oscillator::OscillatorCtrl * mModIdxCtl;

	
	inline void ApplyControls()
	{
		ApplyFreqAndAmpControls();
		if ( mModIdxUpdated )
			{
				mModIndex = mModIdxCtl->GetLastValue();
				mModIdxUpdated = false;
			}
	}
	int UpdateModIdx( TControlData );

public:
	Oscillator();
	
	Oscillator(const OscillatorConfig& c );

	virtual ~Oscillator();
	bool ConcreteConfigure( const ProcessingConfig& c );

	const char * GetClassName() const {return "Oscillator";}
	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do();
	bool Do(Audio & out)
	{
		return SimpleOscillator::Do(out);
	}
	bool Do(const Audio& pitchModIn,const Audio& phaseModIn,Audio& out);
	bool Do(const Audio& pitchModIn,const int& dum ,Audio& out);
	bool Do(const int& dum ,const Audio& phaseModIn,Audio& out);
};

}

#endif //__Oscillator_hxx__ 

