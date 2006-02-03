#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "SimpleOscillator.hxx"
#include "vm_bpf_player.hxx"

namespace CLAM
{
	namespace VM
	{
		const double min_ref = 8.1758;  // 8.1758 Hz 
		const double max_ref = 12545.9; // 12545.9 Hz 

		BPFPlayer::BPFPlayer(QObject* parent)
			: Player(parent)
			, pl_bpf(0)
			, pl_audio(0)
			, pl_pitch_bounds(min_ref,max_ref)
		{
			set_playing_flags(CLAM::VM::eUseOscillator);
		}

		BPFPlayer::~BPFPlayer()
		{
		}

		void BPFPlayer::set_data(const BPF& bpf)
		{
			pl_bpf = &bpf;
		}

		void BPFPlayer::set_audio_ptr(const Audio* audio)
		{
			pl_audio = audio;
		}

		void BPFPlayer::set_pitch_bounds(double min, double max)
		{
			pl_pitch_bounds.min = min;
			pl_pitch_bounds.max = max;
		}

		void BPFPlayer::run()
		{
			if(!pl_bpf) return;
			if(!pl_bpf->Size()) return;
			if(!pl_audio) set_playing_flags(CLAM::VM::eUseOscillator);
			if(pl_audio) pl_sampling_rate = pl_audio->GetSampleRate();

			unsigned frameSize = 512;     
			AudioManager manager((int)pl_sampling_rate,(int)frameSize);  

			AudioOut channel0;   
			AudioIOConfig audioOutCfg0;     
			audioOutCfg0.SetChannelID(0);    
			channel0.Configure(audioOutCfg0); 

			AudioOut channel1;   
			AudioIOConfig audioOutCfg1;     
			audioOutCfg1.SetChannelID(1);    
			channel1.Configure(audioOutCfg1); 
			
			manager.Start();                            
			channel0.Start();  
			channel1.Start();

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(pl_sampling_rate);
			oscCfg.SetAmplitude(0.6);
			SimpleOscillator osc(oscCfg);
			
			InControl& freqControl = osc.GetInControls().Get("Pitch");
    
			Audio samples0;             
			Audio samples1;
			samples0.SetSize(frameSize);
			samples1.SetSize(frameSize);

			Audio silence;
			silence.SetSize(frameSize);

		    unsigned firstIndex = first_index();
			unsigned k = firstIndex;

			unsigned start = unsigned(pl_begin_time*pl_sampling_rate);
			unsigned nSamples = unsigned(pl_time_bounds.max*pl_sampling_rate);
	    
			unsigned leftIndex = start;        
			unsigned rightIndex = leftIndex+frameSize;

			osc.Start();

			for(unsigned i=start; i < nSamples; i+=frameSize)
			{
				if(!pl_bpf->Size()) pl_is_playing = false;
				if(!pl_is_playing) break;
				if(k < (unsigned)pl_bpf->Size()-1) if(double(i)/pl_sampling_rate >= double(pl_bpf->GetXValue(k+1))) k++;
				if(pl_audio)
				{
					if(rightIndex < unsigned(pl_audio->GetSize()))
					{
						pl_audio->GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
					}
				}
				if(k < (unsigned)pl_bpf->Size()) freqControl.DoControl(get_pitch(k));
				osc.Do(samples1);
				if(leftIndex/pl_sampling_rate >= pl_bpf->GetXValue(k))
				{
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
						if(pl_playing_flags & CLAM::VM::eAudio)
						{
							channel0.Do(samples0);
							channel1.Do(samples0);
						}
						else
						{
							channel0.Do(samples1);
							channel1.Do(samples1);
						}
					}
				}
				else
				{
					channel0.Do(silence);
					channel1.Do(silence);
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

		unsigned BPFPlayer::first_index()
		{
			unsigned nPoints = pl_bpf->Size();
			if(nPoints <= 1) return 0;

			if(pl_begin_time <= pl_bpf->GetXValue(0)) return 0;
			if(pl_begin_time >= pl_bpf->GetXValue(nPoints-1)) return nPoints-1;

			unsigned index = 0;
			unsigned left_index = 0;
			unsigned right_index = nPoints-1;
			while(left_index <= right_index)
			{
				const unsigned currentIndex = (left_index+right_index)/2;
				if(currentIndex >= nPoints-1)
				{
					index = currentIndex;
					break;
				}
				if(pl_begin_time >= double(pl_bpf->GetXValue(currentIndex)) && 
				   pl_begin_time <= double(pl_bpf->GetXValue(currentIndex+1)))
				{
					index = currentIndex;
					break;
				}
				if(pl_begin_time < double(pl_bpf->GetXValue(currentIndex)))
				{
					right_index = currentIndex-1;
				}
				else if(pl_begin_time > double(pl_bpf->GetXValue(currentIndex)))
				{
					left_index = currentIndex+1;
				}
			}
			return index;
		}

		double BPFPlayer::get_pitch(unsigned index)
		{
			double value = pl_bpf->GetValueFromIndex(index);
			if(value >= min_ref && value <= max_ref) return value;
			if(value < pl_pitch_bounds.min) value = pl_pitch_bounds.min;
			else if(value > pl_pitch_bounds.max) value = pl_pitch_bounds.max;
			return (value-pl_pitch_bounds.min)*(max_ref-min_ref)/pl_pitch_bounds.span()+min_ref;
		}
	}
}

// END
