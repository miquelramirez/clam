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

#include "Oscillator.hxx"
#include <iostream>

using namespace CLAM;

// Oscillator controls enumeration

Enum::tEnumValue EOscillatorControls::sEnumValues[] =
	{
		{ EOscillatorControls::pitch, "pitch" },
		{ EOscillatorControls::amplitude, "amplitude" },
		{ EOscillatorControls::modidx, "modidx" },
		{ EOscillatorControls::phase, "phase" },
		{ 0, NULL }
	};

Enum::tValue EOscillatorControls::sDefault = EOscillatorControls::pitch;

// OscillatorConfig method definition
void OscillatorConfig::DefaultInit(void)
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


// Oscillator method definition
Oscillator::Oscillator()
	:mOutput("Audio Output",this,1)
	,mFreqUpdated( false )
	,mPhaseUpdated( false )
	,mModIdxUpdated( false )
	,mAmpUpdated( false )

{
	mFreqCtl = new OscillatorCtrl( "Pitch", this, &Oscillator::UpdateFreq );
	mAmpCtl = new OscillatorCtrl( "Amplitude", this, &Oscillator::UpdateAmp );
	mModIdxCtl = new OscillatorCtrl( "ModIndex", this, &Oscillator::UpdateModIdx );
	mPhaseCtl = new OscillatorCtrl( "Phase", this, &Oscillator::UpdatePhase );

	OscillatorConfig cfg;

	Configure( cfg );
}

Oscillator::Oscillator( const OscillatorConfig& cfg )
:mOutput("Audio Output",this,1)
,mFreqUpdated( false )
,mPhaseUpdated( false )
,mModIdxUpdated( false )
,mAmpUpdated( false )

{
	mFreqCtl = new OscillatorCtrl( "Pitch", this, &Oscillator::UpdateFreq );
	mAmpCtl = new OscillatorCtrl( "Amplitude", this, &Oscillator::UpdateAmp );
	mModIdxCtl = new OscillatorCtrl( "ModIndex", this, &Oscillator::UpdateModIdx );
	mPhaseCtl = new OscillatorCtrl( "Phase", this, &Oscillator::UpdatePhase );

	Configure( cfg );
}

Oscillator::~Oscillator()
{
	delete mFreqCtl;
	delete mAmpCtl;
	delete mModIdxCtl;
	delete mPhaseCtl;
}

bool Oscillator::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);


	mAmp = mConfig.GetAmplitude();
	mPhase = mConfig.GetPhase(); // TEMP HACK  (See also constructor
	mModIndex = mConfig.GetModIndex();
	mSamplingRate = mConfig.GetSamplingRate();
	mDeltaPhase = TData(2.*PI*mConfig.GetFrequency()/mSamplingRate);

	return true;
}

bool Oscillator::Do()
{
	bool res = Do(mOutput.GetData());
	mOutput.LeaveData();
	return res;
}

bool Oscillator::Do( Audio& out )
{
	if( !AbleToExecute() ) return true;
	
	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase + mModIndex));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2*PI)) 
			mPhase-=TData(2*PI);
	}

	return true;
}

bool Oscillator::Do( const Audio& pitchModIn, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase + mModIndex*(*phaseModptr++)));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>2.*PI) 
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}

	return true;
}

bool Oscillator::Do( const Audio& pitchModIn, const int& dum, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* pitchModptr = pitchModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase));
		mPhase += mDeltaPhase*(*pitchModptr++);
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
		
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	return true;
}

bool Oscillator::Do( const int& dum, const Audio& phaseModIn, Audio& out )
{
	if( !AbleToExecute() ) return true;

	ApplyControls();

	TData* ptr = out.GetBuffer().GetPtr();
	TData* phaseModptr = phaseModIn.GetBuffer().GetPtr();
	
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase + mModIndex*(*phaseModptr++)));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2.*PI) )
			mPhase-=TData(2.*PI);
			
		if (mPhase<0) 
			mPhase+=TData(2.*PI);
	}
	
	return true;
}

int Oscillator::UpdateFreq( TControlData value )
{
	mFreqUpdated = true;

	return 0;
}

int Oscillator::UpdatePhase( TControlData value )
{
	mPhaseUpdated = true;

	return 0;
}

int Oscillator::UpdateModIdx( TControlData value )
{
	mModIdxUpdated = true;

	return 0;
}

int Oscillator::UpdateAmp( TControlData value )
{
	mAmpUpdated = true;

	return 0;
}
