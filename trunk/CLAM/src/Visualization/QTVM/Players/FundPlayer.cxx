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
			
			PublishedInControls controls = osc.GetInControls();
			InControl& freqControl = controls.Get("Pitch");
    
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
				leftIndex += frameSize;
				rightIndex += frameSize;
			 }
			 osc.Stop();
			 channel.Stop(); 
			 if(!IsPaused()) _time.SetBegin(GetBeginTime());
		}
	}
}

// END
