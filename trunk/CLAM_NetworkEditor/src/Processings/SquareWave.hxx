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

#ifndef _SquareWave_
#define _SquareWave_

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "OSDefines.hxx"
#include "Audio.hxx"
#include "AudioOutPort.hxx"
#include "InControl.hxx"
#include "Enum.hxx"
#include <string>

#define SGN(x) ((x<0)?(-1):(1))

namespace CLAM
{

class ESquareWaveControls : public Enum
{
public:

	static tEnumValue sEnumValues[];
	static tValue sDefault;

	ESquareWaveControls() 
		: Enum(sEnumValues, sDefault) 
	{
	}
	
	ESquareWaveControls(tValue v)
		: Enum(sEnumValues, v) 
	{
	}
	
	ESquareWaveControls(std::string s)
		: Enum(sEnumValues, s)
	{
	}
	
	~ESquareWaveControls() 
	{
	};

	Component * Species() const 
	{
		return new ESquareWaveControls;
	}

	typedef enum 
		{
			pitch=0,
			amplitude,
			modidx,
			phase
		} tEnum;

};

class SquareWaveConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SquareWaveConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData, Amplitude);
	DYN_ATTRIBUTE (2, public, TData, Phase);
	DYN_ATTRIBUTE (3, public, TData, SamplingRate);
protected:
	void DefaultInit(void);
};
	
class SquareWave: public Processing
{
protected:
	AudioOutPort mOutput;
	SquareWaveConfig mConfig;
	TData mAmp;
	TData mPhase;
	TData mDeltaPhase;
	TData mSamplingRate;

	typedef InControlTmpl<SquareWave> SquareWaveCtrl;

	SquareWaveCtrl* mFreqCtl;
	SquareWaveCtrl* mAmpCtl;

	bool           mFreqUpdated;
	bool           mAmpUpdated;

protected:

	inline void ApplyControls()
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

	SquareWave();
	
	SquareWave(const SquareWaveConfig& c );

	virtual ~SquareWave();

	const char * GetClassName() const {return "SquareWave";}
	
	virtual const ProcessingConfig &GetConfig() const { return mConfig;}
	
	virtual bool ConcreteConfigure(const ProcessingConfig& c);

	virtual bool Do(void);

	// "Generative Do"
	bool Do(Audio& out);
};

}

#endif



