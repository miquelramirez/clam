/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _Oscillator_
#define _Oscillator_

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "OSDefines.hxx"
#include "Audio.hxx"
#include "OutPortTmpl.hxx"
#include "InControl.hxx"
#include "Enum.hxx"
#include <string>

namespace CLAM
{

class EOscillatorControls : public Enum
{
public:

	static tEnumValue sEnumValues[];
	static tValue sDefault;
	
	EOscillatorControls() 
		: Enum(sEnumValues, sDefault) 
	{
	}
	
	EOscillatorControls(tValue v) 
		: Enum(sEnumValues, v) 
	{
	}
	
	EOscillatorControls(std::string s) 
		: Enum(sEnumValues, s) 
	{
	}
	
	~EOscillatorControls() 
	{
	};

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

};

class OscillatorConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OscillatorConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, TData, Frequency);
	DYN_ATTRIBUTE (2, public, TData, Amplitude);
	DYN_ATTRIBUTE (3, public, TData, ModIndex);
	DYN_ATTRIBUTE (4, public, TData , Phase);
	DYN_ATTRIBUTE (5, public, TData , SamplingRate);
protected:
	void DefaultInit(void);
};
	
class Oscillator: public Processing
{

private:
	OutPortTmpl<Audio> mOutput;

	OscillatorConfig mConfig;
	TData mModIndex;
	TData mAmp;
	TData mPhase;
	TData mDeltaPhase;
	TData mSamplingRate;

	typedef InControlTmpl<Oscillator> OscillatorCtrl;

	OscillatorCtrl* mAmpCtl;
	OscillatorCtrl* mPhaseCtl;
	OscillatorCtrl* mFreqCtl;
	OscillatorCtrl* mModIdxCtl;
	
	bool           mFreqUpdated;
	bool           mPhaseUpdated;
	bool           mModIdxUpdated;
	bool           mAmpUpdated;

protected:	

	inline void ApplyControls()
	{
		if ( mFreqUpdated )
			{
				mDeltaPhase = TData(2. * PI * mFreqCtl->GetLastValue() / mSamplingRate);
				mFreqUpdated = false;
			}
		else if ( mAmpUpdated )
			{
				mAmp = mAmpCtl->GetLastValue();
				mAmpUpdated = false;
			}
		else if ( mModIdxUpdated )
			{
				mModIndex = mModIdxCtl->GetLastValue();
				mModIdxUpdated = false;
			}
		else if ( mPhaseUpdated )
			{
				mPhase = mPhaseCtl->GetLastValue();
				mPhaseUpdated = false;
			}
	}

	int UpdateFreq( TControlData );

	int UpdatePhase( TControlData );
	
	int UpdateModIdx( TControlData );

	int UpdateAmp( TControlData );

public:

	Oscillator();
	
	Oscillator(const OscillatorConfig& c );

	virtual ~Oscillator();

	const char * GetClassName() const {return "Oscilator";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	// Unsupervised mode Do
	// now it works only for an audio output and no inputs
	bool Do(void); //{ return true; }

	// "Generative Do"
	bool Do(Audio& out);
	
	bool Do(const Audio& pitchModIn,const Audio& phaseModIn,Audio& out);
	
	bool Do(const Audio& pitchModIn,const int& dum ,Audio& out);
		
	bool Do(const int& dum ,const Audio& phaseModIn,Audio& out);
	
	/*
	bool Do(const Audio& modIn,Audio& out)
	{
		TData* ptr = out.GetBuffer().GetPtr();
		TData* modptr = modIn.GetBuffer().GetPtr();
		for (int i=0;i<out.GetSize();i++)
		{
			(*ptr++) = sin(mPhase + mModIndex*(*modptr++));
			mPhase += mDeltaPhase;
			if (mDeltaPhase>2.*PI) mDeltaPhase-=2.*PI;
			if (mDeltaPhase<0) mDeltaPhase+=2.*PI;
		}
		return true;
	}
	*/

	void StoreOn(Storage &s) {};
};

}

#endif


