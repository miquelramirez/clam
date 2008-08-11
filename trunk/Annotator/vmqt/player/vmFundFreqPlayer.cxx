/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Segment.hxx"
#include "AudioIO.hxx" 
#include "AudioManager.hxx" 
#include "AudioOut.hxx" 
#include "SimpleOscillator.hxx" 
#include "vmFundFreqPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		FundFreqPlayer::FundFreqPlayer(QObject* parent)
			: Player(parent)
			, mSegment(0)
		{
			SetPlayingFlags(CLAM::VM::eUseOscillator);
		}

		FundFreqPlayer::~FundFreqPlayer()
		{
		}

		void FundFreqPlayer::SetData(const Segment& segment)
		{
			mSegment = &segment;
		}

		void FundFreqPlayer::run()
		{
			if(!mSegment) return;
			if(!mSegment->GetnFrames()) return;
			if(!mSegment->HasAudio()) mPlayingFlags = CLAM::VM::eUseOscillator;

			mSamplingRate = mSegment->GetSamplingRate(); 
			unsigned nSamples = unsigned((mSegment->GetEndTime()-mSegment->GetBeginTime())*mSamplingRate);
			int frameSize = 1024;                    

			AudioManager manager(mSamplingRate,frameSize);  
			AudioOut channel0;   
			AudioOut channel1;
			AudioIOConfig audioOutCfg0;     
			AudioIOConfig audioOutCfg1; 
			audioOutCfg0.SetChannelID(0);    
			audioOutCfg1.SetChannelID(1);
			channel0.Configure(audioOutCfg0); 
			channel1.Configure(audioOutCfg1);

			manager.Start();                            
			channel0.Start();              
			channel1.Start();
    
			Audio samples0;  
			Audio samples1;
			samples0.SetSize(frameSize);
			samples1.SetSize(frameSize);

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(mSamplingRate);
			oscCfg.SetAmplitude(0.6);
			SimpleOscillator osc(oscCfg);
			
			int nFrames = mSegment->GetnFrames();

			osc.Start();

			unsigned leftIndex = unsigned(mBeginTime*mSamplingRate);
			unsigned rightIndex = leftIndex+frameSize;

			while(leftIndex < unsigned(mTimeBounds.max*mSamplingRate))
			{
				if (mPlayStatus!=Playing) break;
				if (mPlayingFlags & CLAM::VM::eAudio)
				{
					mSegment->GetAudio().GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
				}
				SendFloatToInControl(osc, "Pitch",mSegment->GetFrame(int(leftIndex*nFrames/nSamples)).GetFundamental().GetFreq(0)); 
				osc.Do(samples1);		
				if((mPlayingFlags & CLAM::VM::eAudio) && (mPlayingFlags & CLAM::VM::eUseOscillator))
				{
					channel0.Do(samples0);
					channel1.Do(samples1);
				}
				else if(mPlayingFlags & CLAM::VM::eAudio)
				{
					channel0.Do(samples0);
					channel1.Do(samples0);
				}
				else
				{
					channel0.Do(samples1);
					channel1.Do(samples1);
				}
				emit playingTime(double(leftIndex)/mSamplingRate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			osc.Stop();
			channel0.Stop();
			channel1.Stop();

			if (mPlayStatus==Playing) mPlayStatus=Stoped;
			mBeginTime = (mPlayStatus==Paused) ? double(leftIndex)/mSamplingRate : mTimeBounds.min;
			emit stopTime(double(leftIndex)/mSamplingRate,mPlayStatus==Paused);
		}
	}
}

// END

