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

#ifndef __ADSR__
#define __ADSR__

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "Audio.hxx"
#include "AudioIO.hxx"
#include "OSDefines.hxx"
#include "InControl.hxx"

namespace CLAM
{
	class ADSRConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ADSRConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TData, AttackTime);
		DYN_ATTRIBUTE (2, public, TData, DecayTime);
		DYN_ATTRIBUTE (3, public, TData, SustainLevel);
		DYN_ATTRIBUTE (4, public, TData , ReleaseTime);
		DYN_ATTRIBUTE (5, public, TData , SamplingRate);
	protected:
		void DefaultInit(void);
	};

	class ADSR: public Processing
	{
	private:
		ADSRConfig             mConfig;
		TData                  mAttackTime;
		TData                  mDecayTime;
		TData                  mSustainLevel;
		TData                  mReleaseTime;
		TData                  mSamplingRate;
		TData                  mLevel;
		TData                  mDLevel;
		TControlData           mAmpValue;
		InControlTmpl< ADSR >  mAmplitude;
		OutControl             mState;

		enum Status {
		Attack = 0,
		Decay = 1,
		Sustain = 2,
		Release = 3,
		Done = 4,
		} mStatus;

	protected:
		
		void HandleAttack(void);

		void HandleDecay(void);

		void HandleRelease(void);
		
		void HandleAmplitude(void)
		{
			if ( mAmpValue > 0 )
				HandleAttack();
				
			if ( mAmpValue == 0 )
				HandleRelease();			
		}

		void UpdateState(void)
		{
			if( mStatus == Done )
				mState.SendControl( 0 );  // Available instrument
			else
				mState.SendControl( 1 );  // Busy intrument
		}

		int UpdateAmp( TControlData value )
		{
			mAmpValue = value ;
			HandleAmplitude();
			return 0;
		}

	public:

		ADSR();

		ADSR( const ADSRConfig& c );

		~ADSR(){}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);

		// Unsupervised mode 
		bool Do(void) { return true; }

		bool Do( Audio& out);

	};
}

#endif
