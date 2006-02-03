#include "Segment.hxx"
#include "AudioIO.hxx" 
#include "AudioManager.hxx" 
#include "AudioOut.hxx" 
#include "SimpleOscillator.hxx" 
#include "vm_fund_freq_player.hxx"

namespace CLAM
{
	namespace VM
	{
		FundFreqPlayer::FundFreqPlayer(QObject* parent)
			: Player(parent)
			, pl_segment(0)
		{
			set_playing_flags(CLAM::VM::eUseOscillator);
		}

		FundFreqPlayer::~FundFreqPlayer()
		{
		}

		void FundFreqPlayer::set_data(const Segment& segment)
		{
			pl_segment = &segment;
		}

		void FundFreqPlayer::run()
		{
			if(!pl_segment) return;
			if(!pl_segment->GetnFrames()) return;
			
			if(!pl_segment->HasAudio()) pl_playing_flags = CLAM::VM::eUseOscillator;

			pl_sampling_rate = pl_segment->GetSamplingRate(); 
			unsigned nSamples = unsigned((pl_segment->GetEndTime()-pl_segment->GetBeginTime())*pl_sampling_rate);
			int frameSize = 512;                    

			AudioManager manager((int)pl_sampling_rate,frameSize);  
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
			oscCfg.SetSamplingRate(pl_sampling_rate);
			oscCfg.SetAmplitude(0.6);
			SimpleOscillator osc(oscCfg);
			
			InControl& freqControl = osc.GetInControls().Get("Pitch");

			int nFrames = pl_segment->GetnFrames();

			osc.Start();

			unsigned leftIndex = unsigned(pl_begin_time*pl_sampling_rate);
			unsigned rightIndex = leftIndex+frameSize;

			while(leftIndex < unsigned(pl_time_bounds.max*pl_sampling_rate))
			{
				if(!pl_is_playing) break;
				if(pl_playing_flags & CLAM::VM::eAudio)
				{
					pl_segment->GetAudio().GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
				}
				freqControl.DoControl(pl_segment->GetFrame(int(leftIndex*nFrames/nSamples)).GetFundamental().GetFreq(0));
				osc.Do(samples1);		
				if((pl_playing_flags & CLAM::VM::eAudio) && (pl_playing_flags & CLAM::VM::eUseOscillator))
				{
					channel0.Do(samples0);
					channel1.Do(samples1);
				}
				else if(pl_playing_flags & CLAM::VM::eAudio)
				{
					channel0.Do(samples0);
					channel1.Do(samples0);
				}
				else
				{
					channel0.Do(samples1);
					channel1.Do(samples1);
				}			   
				emit playingTime(double(leftIndex)/pl_sampling_rate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			osc.Stop();
			channel0.Stop();
			channel1.Stop();

			(pl_is_paused) ? pl_begin_time = double(leftIndex)/pl_sampling_rate : pl_begin_time = pl_time_bounds.min;
			emit stopTime(double(leftIndex)/pl_sampling_rate,pl_is_paused);
			pl_is_playing = false;
		}
	}
}

// END
