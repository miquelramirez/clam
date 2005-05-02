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

#include "AudioIO.hxx" 
#include "AudioManager.hxx" 
#include "AudioOut.hxx" 
#include "SimpleOscillator.hxx" 
#include "FundPlayer.hxx" 

namespace CLAM
{
	namespace VM
	{
		FundPlayer::FundPlayer()
		{
		}
		
		FundPlayer::~FundPlayer()
		{
		}
		
		void FundPlayer::SetData(const Segment& segment)
		{
			_segment = segment;
			MediaTime time;
			time.SetBegin(TData(_segment.GetBeginTime()));
			time.SetEnd(TData(_segment.GetEndTime()));
			SetBounds(time);
			_thread.SetThreadCode(makeMemberFunctor0((*this), FundPlayer, thread_code));
			HaveData(true);
		}
		
		void FundPlayer::thread_code()
		{
			TData sampleRate = _segment.GetSamplingRate(); 
			TSize nSamples = TSize((_segment.GetEndTime()-_segment.GetBeginTime())*sampleRate);         
			TSize frameSize = 512;                    

			AudioManager manager((int)sampleRate,(int)frameSize);  
			AudioOut channel;   
			AudioIOConfig audioOutCfg;     
			audioOutCfg.SetChannelID(0);    
			channel.Configure(audioOutCfg); 
			
			AudioManager::Current().Start();                            
			channel.Start();  

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(sampleRate);
			oscCfg.SetAmplitude(TData(0.6));
			SimpleOscillator osc(oscCfg);
			
			InControl& freqControl = osc.GetInControls().Get("Pitch");
    
			Audio samples;                
			samples.SetSize(frameSize);
																		
			int nFrames = _segment.GetnFrames();

			osc.Start();

			TIndex leftIndex = TIndex(_time.GetBegin()*sampleRate);        
			TIndex rightIndex = leftIndex+frameSize;

			while(leftIndex < TIndex(_time.GetEnd()*sampleRate))
			{
				if(IsPaused())
				{
					_time.SetBegin(TData(leftIndex)/sampleRate);
					SetPlaying(false);
				}
				if(!IsPlaying()) break;
				freqControl.DoControl(_segment.GetFrame(int(leftIndex*nFrames/nSamples)).GetFundamental().GetFreq(0));
				osc.Do(samples);
				channel.Do(samples);

				mSigPlayingTime.Emit(TData(leftIndex)/sampleRate);

				leftIndex += frameSize;
				rightIndex += frameSize;
			 }
			 osc.Stop();
			 channel.Stop(); 
			 if(!IsPaused()) _time.SetBegin(GetBeginTime());

			 TData stopTime = (IsStopped()) ? TData(leftIndex)/sampleRate : (IsPaused()) ? _time.GetBegin() : _time.GetEnd();
			 mSigStop.Emit(stopTime);
		}
	}
}

// END
