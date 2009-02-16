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

#include "Oscillator.hxx"
#include "ProcessingFactory.hxx"


namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Oscillator",
		"category", "Generators",
		"description", "Oscillator",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Oscillator> reg = metadata;
}

void OscillatorConfig::DefaultInit(void)
{
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


Oscillator::Oscillator(const OscillatorConfig& c )
	: mInputPhaseMod("Input Phase Modulation", this )
	, mInputFreqMod("Input Frequency Modulation", this )
	, mModIdxUpdated( false )
	, mModIdxCtl("ModIndex", this, &Oscillator::UpdateModIdx )
{	

	SimpleOscillatorConfig simpleCfg;
	simpleCfg.SetFrequency( c.GetFrequency() );
	simpleCfg.SetAmplitude( c.GetAmplitude() );
	simpleCfg.SetSamplingRate( c.GetSamplingRate() );

	Configure( c );
}

Oscillator::~Oscillator()
{
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
	bool res =Do(mInputFreqMod.GetAudio(),mInputPhaseMod.GetAudio(),mOutput.GetAudio());
	mInputFreqMod.Consume();
	mInputPhaseMod.Consume();
	mOutput.Produce();
	return res;
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

void Oscillator::UpdateModIdx( TControlData value )
{
	mModIdxUpdated = true;
}


} // namespace CLAM

