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

#include "SimpleOscillator.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SimpleOscillator",
		"category", "Generators",
		"description", "SimpleOscillator",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SimpleOscillator> reg = metadata;
}

	
// OscillatorConfig method definition
void SimpleOscillatorConfig::DefaultInit(void)
{
	AddFrequency();
	AddAmplitude();
	AddPhase();
	AddSamplingRate();
	
	UpdateData();

	SetFrequency(440.0);
	SetAmplitude(1.0);
	SetPhase(0.0);
	SetSamplingRate( 44100 );
}


// Oscillator method definition
SimpleOscillator::SimpleOscillator()
	:mOutput("Audio Output", this),
	mFreqUpdated( false ),
	mAmpUpdated( false ),
	mFreqCtl(0),
	mAmpCtl(0),
	mSamplesBetweenCallsCtl("SamplesBetweenCalls", this)

{
	mFreqCtl = new SimpleOscillatorCtrl( "Pitch", this, &SimpleOscillator::UpdateFreq );
	mAmpCtl = new SimpleOscillatorCtrl( "Amplitude", this, &SimpleOscillator::UpdateAmp );

	SimpleOscillatorConfig cfg;

	Configure( cfg );
}

SimpleOscillator::SimpleOscillator( const SimpleOscillatorConfig& cfg )
	:mOutput("Audio Output", this),
	mFreqUpdated( false ),
	mAmpUpdated( false ),
	mFreqCtl(0),
	mAmpCtl(0),
	mSamplesBetweenCallsCtl("SamplesBetweenCalls", this)
{
	mFreqCtl = new SimpleOscillatorCtrl( "Pitch", this, &SimpleOscillator::UpdateFreq );
	mAmpCtl = new SimpleOscillatorCtrl( "Amplitude", this, &SimpleOscillator::UpdateAmp );
	Configure( cfg );
}

SimpleOscillator::~SimpleOscillator()
{
	delete mFreqCtl;
	delete mAmpCtl;
}

bool SimpleOscillator::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);


	mAmp = mConfig.GetAmplitude();
	mPhase = mConfig.GetPhase(); // TEMP HACK  (See also constructor
	mSamplingRate = mConfig.GetSamplingRate();
	mDeltaPhase = TData(2.*PI*mConfig.GetFrequency()/mSamplingRate);
	//xamat: kludge to convert this into an LFO, eventually separate into a different class
	mSamplesBetweenCallsCtl.DoControl(1);
	return true;
}

bool SimpleOscillator::Do()
{
	bool res = false;
	res = Do(mOutput.GetAudio());
	mOutput.Produce();
	return res;
}

bool SimpleOscillator::Do( Audio& out )
{
	if( !AbleToExecute() ) return true;
	
	ApplyFreqAndAmpControls();

	TData* ptr = out.GetBuffer().GetPtr();
	for (int i=0;i<out.GetSize();i++)
	{
		(*ptr++) = mAmp * TData(sin(mPhase));
		mPhase += mDeltaPhase;
		
		if (mPhase>TData(2*PI)) 
			mPhase-=TData(2*PI);
	}

	return true;
}

//xamat: kludge to convert this into an LFO, eventually separate into a different class
bool SimpleOscillator::Do( TData& out )
{
	if( !AbleToExecute() ) return true;
	
	ApplyFreqAndAmpControls();

	out = mAmp * TData(sin(mPhase));
	mPhase += mDeltaPhase*mSamplesBetweenCallsCtl.GetLastValue();
		
	if (mPhase>TData(2*PI)) 
		mPhase-=TData(2*PI);

	return true;
}


int SimpleOscillator::UpdateFreq( TControlData value )
{
	mFreqUpdated = true;

	return 0;
}

int SimpleOscillator::UpdateAmp( TControlData value )
{
	mAmpUpdated = true;

	return 0;
}

} // namespace CLAM

