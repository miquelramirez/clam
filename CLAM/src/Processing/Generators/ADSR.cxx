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


#include "ADSR.hxx"
#include "AudioManager.hxx"

using namespace CLAM;

void ADSRConfig::DefaultInit(void)
{
	AddName();
	AddAttackTime();
	AddDecayTime(),
	AddSustainLevel();
	AddReleaseTime();
	AddSamplingRate();

	UpdateData();

	try
	{
		SetSamplingRate( AudioManager::Current().SampleRate() );
	}
	catch(Err)
	{
		SetSamplingRate( 8000 );
	}

}

ADSR::ADSR(): 
mAmpValue(0),
mLevel(0),
mDLevel(0),
mStatus(Done),
mState( "State", this ),
mAmplitude( "Amplitude", this, &ADSR::UpdateAmp )
{
	ADSRConfig cfg;

	Configure( cfg );
}

ADSR::ADSR( const ADSRConfig& cfg ):
mAmpValue(0),
mLevel(0),
mDLevel(0),
mStatus(Done),
mState( "State", this ),
mAmplitude( "Amplitude", this, &ADSR::UpdateAmp )
{
	Configure( cfg );
}

bool ADSR::ConcreteConfigure( const ProcessingConfig& c )
{
	CopyAsConcreteConfig(mConfig, c);

	mAttackTime = mConfig.GetAttackTime();
	mDecayTime = mConfig.GetDecayTime();
	mSustainLevel = mConfig.GetSustainLevel();
	mReleaseTime = mConfig.GetReleaseTime();
	mSamplingRate = mConfig.GetSamplingRate();

	return true;
}

void ADSR::HandleAttack(void)
{
	mStatus = Attack;
	mLevel = 0;
	mDLevel = mAmpValue / ( mAttackTime * mSamplingRate );
}

void ADSR::HandleDecay(void)
{
	mStatus = Decay;
	mDLevel = ( mSustainLevel * mAmpValue - mAmpValue ) / ( mDecayTime * mSamplingRate ) ;
}

void ADSR::HandleRelease(void)
{
	mStatus = Release;
	mDLevel = - mLevel / ( mReleaseTime * mSamplingRate );
}

bool ADSR::Do( Audio& out)
{
/*** this is the "clean" version:

	TData* ptr = out.GetBuffer().GetPtr();

	for ( int i = 0; i < out.GetSize() ; i++ )
	{
		(*ptr++) = mLevel ;
		mLevel += mDLevel ;

		if ( ( mStatus == Attack ) && ( mLevel >= mAmpValue ) )
		{
			mStatus = Decay ;
			HandleDecay();
		}
		else if ( ( mStatus == Decay ) && ( mLevel <= mSustainLevel ) )
		{
			mStatus = Sustain ;
			mDLevel = 0 ;
		}
		else if ( ( mStatus == Release ) && ( mLevel <= 0 ) )
		{
			mDLevel = 0;
			mLevel = 0;
			mStatus = Done;
		}
	}
*/

/*
 the following does the same, but more efficient, by moving part of the conditions
 outside of the for loop
*/

	TData* ptr = out.GetBuffer().GetPtr();
	TData* endptr = ptr+out.GetSize();

	if (mStatus==Attack)
	{
		while ( ptr!=endptr )
		{
			(*ptr++) = mLevel ;
			mLevel += mDLevel ;

			if (mLevel >= mAmpValue)
			{
				mStatus = Decay ;
				HandleDecay();
				break;
			}
		}
	}
	if (mStatus==Decay)
	{
		while ( ptr!=endptr )
		{
			(*ptr++) = mLevel ;
			mLevel += mDLevel ;

			if (mLevel <= mSustainLevel )
			{
				mStatus = Sustain ;
				mDLevel = 0 ;
				break;
			}
		}
	}
	if (mStatus==Sustain)
	{
		while ( ptr!=endptr )
		{
			(*ptr++) = mLevel ;
		}
	}
	if (mStatus==Release)
	{
		while ( ptr!=endptr )
		{
			(*ptr++) = mLevel ;
			mLevel += mDLevel ;
			
			if (mLevel <=0 )
			{
				mDLevel = 0;
				mLevel = 0;
				mStatus = Done;
				break;
			}
		}
	}
	if (mStatus==Done)
	{
		while ( ptr!=endptr )
		{
			(*ptr++) = 0 ;
		}
	}

	UpdateState();

	return true;
}	
