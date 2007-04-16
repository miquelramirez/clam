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

#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "vmAudioPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		AudioPlayer::AudioPlayer(QObject* parent)
			: Player(parent)
			, mChn0Muted(false)
			, mChn1Muted(false)
			, mChn0(0)
			, mChn1(0)
		{
		}

		AudioPlayer::~AudioPlayer()
		{
		}

		void AudioPlayer::SetData(std::vector<const Audio*> data)
		{
			if(!data.size()) return;
			if(data.size()==1)
			{
				if(!data[0]) return;
				mChn0 = mChn1 = data[0];
			}
			else 
			{
				if(!data[0] && !data[1]) return;
				if(!data[0]) data[0] = data[1];
				if(!data[1]) data[1] = data[0];
				if(data[0]->GetSampleRate() != data[1]->GetSampleRate()) return;
				if(data[0]->GetBuffer().Size() != data[1]->GetBuffer().Size()) return;
				mChn0 = data[0];
				mChn1 = data[1];
			}
		}

		void AudioPlayer::muteChannel0(bool muted)
		{
			mChn0Muted = muted;
		}

		void AudioPlayer::muteChannel1(bool muted)
		{
			mChn1Muted = muted;
		}

		void AudioPlayer::run()
		{
			if(!mChn0 || !mChn1) return;
			if(mTimeBounds.max > mChn0->GetDuration()/1000.0) mTimeBounds.max = mChn0->GetDuration()/1000.0;
			
			mSamplingRate = mChn0->GetSampleRate(); 
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

			Audio silence;
			silence.SetSize(frameSize);

			unsigned leftIndex = unsigned(mBeginTime*mSamplingRate);
			unsigned rightIndex = leftIndex+frameSize;

			while(leftIndex < unsigned(mTimeBounds.max*mSamplingRate))
			{
				if (mPlayStatus!=Playing) break;
				mChn0->GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
				mChn1->GetAudioChunk(int(leftIndex),int(rightIndex),samples1);
				if (!mChn0Muted)
				{
					channel0.Do(samples0);
				}
				else
				{
					channel0.Do(silence);
				}
				if(!mChn1Muted)
				{
					channel1.Do(samples1);
				}
				else
				{
					channel1.Do(silence);
				}
				emit playingTime(double(leftIndex)/mSamplingRate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			channel0.Stop();
			channel1.Stop();

			if (mPlayStatus==Playing) mPlayStatus=Stoped;
			mBeginTime = (mPlayStatus==Paused) ? double(leftIndex)/mSamplingRate : mTimeBounds.min;
			emit stopTime(double(leftIndex)/mSamplingRate,mPlayStatus==Paused);
		}
	}
}

// END

