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

#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/APlayer.hxx>

namespace CLAM
{
    namespace VM
    {
		APlayer::APlayer()
			: mLeftChannel(0)
			, mRightChannel(0)
			, mMuteLeft(false)
			, mMuteRight(false)
		{
			mThread.SetThreadCode(makeMemberFunctor0((*this), APlayer, thread_code));
		}
		
		APlayer::~APlayer()
		{
		}
		
		void APlayer::SetData(std::vector<const Audio*> data, bool setTime)
		{
			if(!data.size()) return;

			if(data.size()==1)
			{
				if(!mLeftChannel) mLeftChannel = data[0];
				mRightChannel = data[0];
			}
			else 
			{
				mLeftChannel = data[0];
				mRightChannel = data[1];
			}

			if(setTime)
			{
				MediaTime time;
				time.SetBegin(TData(0.0));
				time.SetEnd(TData(mLeftChannel->GetSize())/mLeftChannel->GetSampleRate());
				SetBounds(time);
			}	
			HaveData(true);
		}
		
		void APlayer::thread_code()
		{       
			if(!mLeftChannel || !mRightChannel) return;

			TData sampleRate = mLeftChannel->GetSampleRate(); 
			TSize frameSize = 512;                    

			AudioManager manager((int)sampleRate,(int)frameSize);  
			AudioOut channelL;   
			AudioOut channelR;
			AudioIOConfig audioOutCfgL;     
			AudioIOConfig audioOutCfgR; 
			audioOutCfgL.SetChannelID(0);    
			audioOutCfgR.SetChannelID(1);
			channelL.Configure(audioOutCfgL); 
			channelR.Configure(audioOutCfgR);
			manager.Start();                            
			channelL.Start();              
			channelR.Start();
    
			Audio samplesL;  
			Audio samplesR;
			samplesL.SetSize(frameSize);
			samplesR.SetSize(frameSize);

			Audio silence;
			silence.SetSize(frameSize);
								
			TIndex leftIndex = TIndex(mTime.GetBegin()*sampleRate);        
			TIndex rightIndex = leftIndex+frameSize;

			while(leftIndex < TIndex(mTime.GetEnd()*sampleRate))
			{
				if(IsPaused())
				{
					mTime.SetBegin(TData(leftIndex)/sampleRate);
					SetPlaying(false);
				}
				if(!IsPlaying()) break;
				mLeftChannel->GetAudioChunk(leftIndex,rightIndex,samplesL);
				mRightChannel->GetAudioChunk(leftIndex,rightIndex,samplesR);
				if(!IsMutedLChannel())
				{
					channelL.Do(samplesL);
				}
				else
				{
					channelL.Do(silence);
				}
				if(!IsMutedRChannel())
				{
					channelR.Do(samplesR);
				}
				else
				{
					channelR.Do(silence);
				}
				mSigPlayingTime.Emit(TData(leftIndex)/sampleRate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			channelL.Stop(); 
			channelR.Stop();
			if(!IsPaused()) mTime.SetBegin(GetBeginTime());

			TData stopTime = (IsStopped()) ? TData(leftIndex)/sampleRate : (IsPaused()) ? mTime.GetBegin() : mTime.GetEnd();
			mSigStop.Emit(stopTime);
		}

		void APlayer::SetLeftChannelMuted(bool b)
		{
			mMuteLeft = b;
		}

		void APlayer::SetRightChannelMuted(bool b)
		{
			mMuteRight=b;
		}

		const bool& APlayer::IsMutedLChannel() const
		{
			return mMuteLeft;
		}

		const bool& APlayer::IsMutedRChannel() const
		{
			return mMuteRight;
		}
    }
}

// END

