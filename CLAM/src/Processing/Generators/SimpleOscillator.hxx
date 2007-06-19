/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SimpleOscillator_hxx__
#define __SimpleOscillator_hxx__

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "OSDefines.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "InControl.hxx"
#include "Enum.hxx"
#include <string>

namespace CLAM
{

class EOscillatorControls : public Enum
{
public:

	EOscillatorControls() : Enum(ValueTable(), pitch) { } 
	EOscillatorControls(tValue v) : Enum(ValueTable(), v) { } 
	EOscillatorControls(std::string s) : Enum(ValueTable(), s) { }
	~EOscillatorControls() { };

	Component * Species() const 
	{ 
		return new EOscillatorControls;
	}
	typedef enum 
	{
		pitch=0,
		amplitude,
		modidx,
		phase
	} tEnum;
	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] =
			{
				{ pitch, "pitch" },
				{ amplitude, "amplitude" },
				{ modidx, "modidx" },
				{ phase, "phase" },
				{ 0, NULL }
			};
		return sEnumValues;
	}
};

class SimpleOscillatorConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SimpleOscillatorConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData, Amplitude);
	DYN_ATTRIBUTE (2, public, TData, Phase);
	DYN_ATTRIBUTE (3, public, TData, SamplingRate);
	
protected:
	void DefaultInit(void);
};
	
class SimpleOscillator : public Processing
{
protected:
	AudioOutPort mOutput;
	SimpleOscillatorConfig mConfig;
	TData mAmp;
	TData mPhase;
	TData mDeltaPhase;
	TData mSamplingRate;

	typedef InControlTmpl<SimpleOscillator> SimpleOscillatorCtrl;

	bool           mFreqUpdated;
	bool           mAmpUpdated;
	SimpleOscillatorCtrl* mFreqCtl;
	SimpleOscillatorCtrl* mAmpCtl;
	
	//xamat: kludge to convert this into an LFO, eventually separate into a different class
	InControl mSamplesBetweenCallsCtl;
protected:	

	inline void ApplyFreqAndAmpControls()
	{
		if ( mFreqUpdated )
			{
				mDeltaPhase = TData(2. * PI * mFreqCtl->GetLastValue() / mSamplingRate);
				mFreqUpdated = false;
			}
		if ( mAmpUpdated )
			{
				mAmp = mAmpCtl->GetLastValue();
				mAmpUpdated = false;
			}
	}

	int UpdateFreq( TControlData );
	int UpdateAmp( TControlData );

public:

	SimpleOscillator();
	
	SimpleOscillator(const SimpleOscillatorConfig& c );

	virtual ~SimpleOscillator();

	const char * GetClassName() const {return "SimpleOscillator";}
	
	virtual const ProcessingConfig &GetConfig() const { return mConfig;}
	
	virtual bool ConcreteConfigure(const ProcessingConfig& c);
	
	virtual bool Do(void); 

	// "Generative Do"
	bool Do(Audio& out);
	/** Do for generating one sample at a time: useful to use the oscillator as a LFO*/
	bool Do(TData& out);
};

}

#endif


