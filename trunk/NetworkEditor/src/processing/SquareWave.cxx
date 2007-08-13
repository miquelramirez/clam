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
#include <CLAM/ProcessingFactory.hxx>


namespace CLAM
{

namespace detail
{
	static const char * metadata[] = {
		"key", "SquareWave",
		"category", "Generators",
		"description", "SquareWave",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SquareWave> regSquareWave("SquareWave");
	static FactoryRegistrator<ProcessingFactory, SquareWave> reg = metadata;
}
	
	

// SquareWave controls enumeration

Enum::tEnumValue ESquareWaveControls::sEnumValues[] =
	{
		{ ESquareWaveControls::pitch, "pitch" },
		{ ESquareWaveControls::amplitude, "amplitude" },
		{ ESquareWaveControls::phase, "phase" },
		{ 0, NULL }
	};

Enum::tValue ESquareWaveControls::sDefault = ESquareWaveControls::pitch;

// SquareWaveConfig method definition
void SquareWaveConfig::DefaultInit(void)
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


// SquareWave method definition
SquareWave::SquareWave()
	:mOutput("Audio Output",this),
	mFreqCtl(0),
	mAmpCtl(0),
	mFreqUpdated( false ),
	mAmpUpdated( false )

{
	mFreqCtl = new SquareWaveCtrl( "Pitch", this, &SquareWave::UpdateFreq );
	mAmpCtl = new SquareWaveCtrl( "Amplitude", this, &SquareWave::UpdateAmp );

	SquareWaveConfig cfg;

	Configure( cfg );
}

SquareWave::SquareWave( const SquareWaveConfig& cfg )
	:mOutput("Audio Output",this),
	mFreqCtl(0),
	mAmpCtl(0),
	mFreqUpdated( false ),
	mAmpUpdated( false )

{
	mFreqCtl = new SquareWaveCtrl( "Pitch", this, &SquareWave::UpdateFreq );
	mAmpCtl = new SquareWaveCtrl( "Amplitude", this, &SquareWave::UpdateAmp );

	Configure( cfg );
}

SquareWave::~SquareWave()
{
	delete mFreqCtl;
	delete mAmpCtl;
}

bool SquareWave::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);


	mAmp = mConfig.GetAmplitude();
	mPhase = mConfig.GetPhase(); // TEMP HACK  (See also constructor
	mSamplingRate = mConfig.GetSamplingRate();
	mDeltaPhase = TData(2.*PI*mConfig.GetFrequency()/mSamplingRate);

	return true;
}

bool SquareWave::Do()
{
	bool res = Do(mOutput.GetAudio());
	mOutput.Produce();
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

int SquareWave::UpdateFreq( TControlData value )
{
	mFreqUpdated = true;

	return 0;
}


int SquareWave::UpdateAmp( TControlData value )
{
	mAmpUpdated = true;

	return 0;
}

} // namespace CLAM



