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

#include "SquareWave.hxx"
#include <iostream>

using namespace CLAM;

// SquareWave controls enumeration

Enum::tEnumValue ESquareWaveControls::sEnumValues[] =
	{
		{ ESquareWaveControls::pitch, "pitch" },
		{ ESquareWaveControls::amplitude, "amplitude" },
		{ ESquareWaveControls::modidx, "modidx" },
		{ ESquareWaveControls::phase, "phase" },
		{ 0, NULL }
	};

Enum::tValue ESquareWaveControls::sDefault = ESquareWaveControls::pitch;

// SquareWaveConfig method definition
void SquareWaveConfig::DefaultInit(void)
{
	AddName();
	AddFrequency();
	AddAmplitude();
	AddModIndex();
	AddPhase();
	AddSamplingRate();
	
	UpdateData();
	
	SetFrequency(440.0);
	SetAmplitude(1.0);
	SetModIndex(1.0);
	SetPhase(0.0);
	SetSamplingRate( 44100 );
}


// SquareWave method definition
SquareWave::SquareWave()
	:mOutput("Audio Output",this,1),
	mFreqUpdated( false ),
	mPhaseUpdated( false ),
	mModIdxUpdated( false ),
	mAmpUpdated( false )

{
	mFreqCtl = new SquareWaveCtrl( "Pitch", this, &SquareWave::UpdateFreq );
	mAmpCtl = new SquareWaveCtrl( "Amplitude", this, &SquareWave::UpdateAmp );
	mModIdxCtl = new SquareWaveCtrl( "ModIndex", this, &SquareWave::UpdateModIdx );
	mPhaseCtl = new SquareWaveCtrl( "Phase", this, &SquareWave::UpdatePhase );

	SquareWaveConfig cfg;

	Configure( cfg );
}

SquareWave::SquareWave( const SquareWaveConfig& cfg )
	:mOutput("Audio Output",this,1),
	mFreqUpdated( false ),
	mPhaseUpdated( false ),
	mModIdxUpdated( false ),
	mAmpUpdated( false )

{
	mFreqCtl = new SquareWaveCtrl( "Pitch", this, &SquareWave::UpdateFreq );
	mAmpCtl = new SquareWaveCtrl( "Amplitude", this, &SquareWave::UpdateAmp );
	mModIdxCtl = new SquareWaveCtrl( "ModIndex", this, &SquareWave::UpdateModIdx );
	mPhaseCtl = new SquareWaveCtrl( "Phase", this, &SquareWave::UpdatePhase );

	Configure( cfg );
}

SquareWave::~SquareWave()
{
	delete mFreqCtl;
	delete mAmpCtl;
	delete mModIdxCtl;
	delete mPhaseCtl;
}

bool SquareWave::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);


	mAmp = mConfig.GetAmplitude();
	mPhase = mConfig.GetPhase(); // TEMP HACK  (See also constructor
	mModIndex = mConfig.GetModIndex();
	mSamplingRate = mConfig.GetSamplingRate();
	mDeltaPhase = TData(2.*PI*mConfig.GetFrequency()/mSamplingRate);

	return true;
}

bool SquareWave::Do()
{
	bool res = false;
	res = Do(mOutput.GetData());
	mOutput.LeaveData();
	return res;
}

bool SquareWave::Do( Audio& out )
{
	if( !AbleToExecute() ) return true;
	
	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(SGN(sin(mPhase)));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2*PI)) 
			mPhase-=TData(2*PI);
	}

	return true;
}

bool SquareWave::Do( const Audio& pitchModIn, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(SGN(sin(mPhase + mModIndex*(*phaseModptr++))));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>2.*PI) 
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}

	return true;
}

bool SquareWave::Do( const Audio& pitchModIn, const int& dum, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(SGN(sin(mPhase)));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	return true;
}

bool SquareWave::Do( const int& dum, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(SGN(sin(mPhase + mModIndex*(*phaseModptr++))));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
			
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	
	return true;
}

int SquareWave::UpdateFreq( TControlData value )
{
	mFreqUpdated = true;

	return 0;
}

int SquareWave::UpdatePhase( TControlData value )
{
	mPhaseUpdated = true;

	return 0;
}

int SquareWave::UpdateModIdx( TControlData value )
{
	mModIdxUpdated = true;

	return 0;
}

int SquareWave::UpdateAmp( TControlData value )
{
	mAmpUpdated = true;

	return 0;
}


