#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "vm_audio_player.hxx"

namespace CLAM
{
	namespace VM
	{
		AudioPlayer::AudioPlayer(QObject* parent)
			: Player(parent)
			, pl_chn0_muted(false)
			, pl_chn1_muted(false)
			, pl_chn0(0)
			, pl_chn1(0)
		{
		}
			
		AudioPlayer::~AudioPlayer()
		{
		}

		void AudioPlayer::set_data(std::vector<const CLAM::Audio*> data)
		{
			if(!data.size()) return;
			if(data.size()==1)
			{
				if(!data[0]) return;
				pl_chn0 = pl_chn1 = data[0];
			}
			else 
			{
				if(!data[0] && !data[1]) return;
				if(!data[0]) data[0] = data[1];
				if(!data[1]) data[1] = data[0];
				if(data[0]->GetSampleRate() != data[1]->GetSampleRate()) return;
				if(data[0]->GetBuffer().Size() != data[1]->GetBuffer().Size()) return;
				pl_chn0 = data[0];
				pl_chn1 = data[1];
			}
		}

		void AudioPlayer::muteChannel0(bool muted)
		{
			pl_chn0_muted = muted;
		}

		void AudioPlayer::muteChannel1(bool muted)
		{
			pl_chn1_muted = muted;
		}

		void AudioPlayer::run()
		{
			if(!pl_chn0 || !pl_chn1) return;
			if(pl_time_bounds.min < 0) pl_time_bounds.min = 0.0;
			if(pl_time_bounds.max > pl_chn0->GetDuration()) pl_time_bounds.max = pl_chn0->GetDuration();
			
			pl_sampling_rate = pl_chn0->GetSampleRate(); 
			int frameSize = 512;                    

			CLAM::AudioManager manager((int)pl_sampling_rate,frameSize);  
			CLAM::AudioOut channel0;   
			CLAM::AudioOut channel1;
			CLAM::AudioIOConfig audioOutCfg0;     
			CLAM::AudioIOConfig audioOutCfg1; 
			audioOutCfg0.SetChannelID(0);    
			audioOutCfg1.SetChannelID(1);
			channel0.Configure(audioOutCfg0); 
			channel1.Configure(audioOutCfg1);

			manager.Start();                            
			channel0.Start();              
			channel1.Start();
    
			CLAM::Audio samples0;  
			CLAM::Audio samples1;
			samples0.SetSize(frameSize);
			samples1.SetSize(frameSize);

			CLAM::Audio silence;
			silence.SetSize(frameSize);

			unsigned leftIndex = unsigned(pl_begin_time*pl_sampling_rate);        
			unsigned rightIndex = leftIndex+frameSize;

			while(leftIndex < unsigned(pl_time_bounds.max*pl_sampling_rate))
			{
				if(!pl_is_playing) break;
				pl_chn0->GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
				pl_chn1->GetAudioChunk(int(leftIndex),int(rightIndex),samples1);
				if(!pl_chn0_muted)
				{
					channel0.Do(samples0);
				}
				else
				{
					channel0.Do(silence);
				}
				if(!pl_chn1_muted)
				{
					channel1.Do(samples1);
				}
				else
				{
					channel1.Do(silence);
				}
				emit playingTime(double(leftIndex)/pl_sampling_rate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			channel0.Stop();
			channel1.Stop();

			(pl_is_paused) ? pl_begin_time = double(leftIndex)/pl_sampling_rate : pl_begin_time = pl_time_bounds.min;
			emit stopTime(double(leftIndex)/pl_sampling_rate,pl_is_paused);
			pl_is_playing = false;
 		}
	}
}

// END
