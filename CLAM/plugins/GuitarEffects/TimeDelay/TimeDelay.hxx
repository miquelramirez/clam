/*
 * Copyright (c) 2009 Hernán Ordiales <audiocode@uint8.com.ar>
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

#ifndef _TimeDelay_
#define _TimeDelay_

#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>

#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>

#include <CLAM/AudioCircularBuffer.hxx>

namespace CLAM {

	/**	\brief Audio delay circular buffer. */
	class DelayBuffer: public AudioCircularBuffer
	{
		TSize mSampleRate;
		TSize mFrameSize;
		TSize mMaxDelay;

		unsigned long mReadyIndex; //index ready to be played
		unsigned long mCurrentSamplesDelay;

	public:
		DelayBuffer(): AudioCircularBuffer()
		{
			mSampleRate=0; mFrameSize=0; mMaxDelay=0;
			mReadyIndex=0; mCurrentSamplesDelay=0;
		}

		void DelayInitParams(TSize _mSampleRate, TSize _mFrameSize, TSize _mMaxDelay)
		{
			if ( mSampleRate!=_mSampleRate || mFrameSize!=_mFrameSize || mMaxDelay!=_mMaxDelay )
			{
				mSampleRate = _mSampleRate;
				mFrameSize = _mFrameSize;
				mMaxDelay = _mMaxDelay;

				SetBufferSize( (mMaxDelay*2)*mSampleRate ); //*2 to allow realtime delay changes
				SetReadSize(mFrameSize);
				SetWriteSize(mFrameSize);
				Init();
	
				mReadyIndex = 0;
			}
		}

		void SetDelay(TSize seconds)
		{
			mCurrentSamplesDelay = seconds*mSampleRate;
		}

		void DelayAdjustment(TSize newDelay)
		{	
			unsigned long newSamplesDelay = newDelay*mSampleRate;
			if (mCurrentSamplesDelay == newSamplesDelay) return;

			if (mCurrentSamplesDelay<newSamplesDelay )
			{
				if (!mReadyIndex>newSamplesDelay) //not ready for playing yet
				{
					mCurrentSamplesDelay = newSamplesDelay;
					return;
				}
				TSize step = newSamplesDelay-mCurrentSamplesDelay;
				DecreaseReadIndex( step ); //more delay
			}
			else
			{
				TSize step = mCurrentSamplesDelay-newSamplesDelay;
				IncreaseReadIndex( step ); //less delay	
			}
			mCurrentSamplesDelay = newSamplesDelay;
		}

		bool ReadyToPlay() {
			if (mReadyIndex>=mCurrentSamplesDelay) return true;
			mReadyIndex += mFrameSize;
			return false;
		}
	};


	/**
	 *	TimeDelay configuration object.
	 */
	class TimeDelayConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( TimeDelayConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, TSize, MaxDelay );

	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMaxDelay(30);
		}

	};

	/**	\brief Simple audio delay processing.
	*
	*	Useful for adjusting the time, for example for synchronization of two sources
	*/
	class TimeDelay: public Processing
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "TimeDelay"; }

	protected:
		/** Ports **/
		AudioInPort mInput;
		AudioOutPort mOutput;

		/** Controls **/
		FloatInControl mDelay;

		DelayBuffer mBuffer;

	public:
		TimeDelay()
			:
			mInput("Input", this),
			mOutput("Output", this),
			mDelay("Delay", this)
		{
			Configure( mConfig );
		}

		~TimeDelay() {}

		bool Do()
		{
			bool result = Do( mInput.GetAudio(), mOutput.GetAudio() );

			mInput.Consume(); 
			mOutput.Produce();

			return result;
		}	

		bool Do(const Audio& in, Audio& out);

	protected:
		/** Configuration **/
		TimeDelayConfig mConfig;

		const ProcessingConfig& GetConfig() const {	return mConfig; }
		bool ConcreteConfigure(const ProcessingConfig& config);

	};



};//namespace CLAM

#endif // _TimeDelay_
