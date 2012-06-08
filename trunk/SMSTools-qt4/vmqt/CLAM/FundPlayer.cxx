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
#include <CLAM/SimpleOscillator.hxx> 
#include <CLAM/FundPlayer.hxx> 

namespace CLAM
{
	namespace VM
	{
		FundPlayer::FundPlayer()
		{
			mThread.SetThreadCode(makeMemberFunctor0((*this), FundPlayer, thread_code));
		}
		
		FundPlayer::~FundPlayer()
		{
		}
		
		void FundPlayer::SetData(const Segment& segment)
		{
			mSegment = segment;
			MediaTime time;
			if(mSegment.HasAudio())
			{
				time.SetBegin(TData(0.0));
				time.SetEnd(TData(mSegment.GetAudio().GetSize())/segment.GetAudio().GetSampleRate());
				SetBounds(time);
			}
			HaveData(true);
		}
		
		void FundPlayer::thread_code()
		{
			TData sampleRate = mSegment.GetSamplingRate(); 
			TSize nSamples = TSize((mSegment.GetEndTime()-mSegment.GetBeginTime())*sampleRate);         
			TSize frameSize = 512;                    

			AudioManager manager((int)sampleRate,(int)frameSize);  
			AudioOut channel;   
			AudioIOConfig audioOutCfg;     
			audioOutCfg.SetChannelID(0);    
			channel.Configure(audioOutCfg); 
			
			manager.Start();                            
			channel.Start();  

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(sampleRate);
			oscCfg.SetAmplitude(TData(0.6));
			SimpleOscillator osc(oscCfg);
			
			Audio samples;                
			samples.SetSize(frameSize);
																		
			int nFrames = mSegment.GetnFrames();

			osc.Start();

			TIndex index = TIndex(mTime.GetBegin()*sampleRate);        

			while(index < TIndex(mTime.GetEnd()*sampleRate))
			{
				if(IsPaused())
				{
					mTime.SetBegin(TData(index)/sampleRate);
					SetPlaying(false);
				}
				if(!IsPlaying()) break;
				SendFloatToInControl(osc, "Pitch", mSegment.GetFrame(int(index*nFrames/nSamples)).GetFundamental().GetFreq(0));
				osc.Do(samples);
				channel.Do(samples);

				mSigPlayingTime.Emit(TData(index)/sampleRate);

				index += frameSize;
			}
			osc.Stop();
			channel.Stop(); 
			if(!IsPaused()) mTime.SetBegin(GetBeginTime());

			TData stopTime = (IsStopped()) ? TData(index)/sampleRate : (IsPaused()) ? mTime.GetBegin() : mTime.GetEnd();
			mSigStop.Emit(stopTime);
		}
	}
}

// END

