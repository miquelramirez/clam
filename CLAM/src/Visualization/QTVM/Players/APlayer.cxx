#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "APlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		APlayer::APlayer()
		{
			_muteLeft=false;
			_muteRight=false;
		}
		
		APlayer::~APlayer()
		{
		}
		
		void APlayer::SetData(std::vector<Audio> data)
		{
			if(data.size()==1)
			{
				_leftChannel = data[0];
				_rightChannel = data[0];
			}
			else
			{
				_leftChannel = data[0];
				_rightChannel = data[1];
			}
			MediaTime time;
			time.SetBegin(TData(0.0));
			time.SetEnd(TData(_leftChannel.GetSize())/_leftChannel.GetSampleRate());
			SetBounds(time);
			_thread.SetThreadCode(makeMemberFunctor0((*this), APlayer, thread_code));
			HaveData(true);
		}
		
		void APlayer::thread_code()
		{       
			TData sampleRate = _leftChannel.GetSampleRate(); 
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
			AudioManager::Current().Start();                            
			channelL.Start();              
			channelR.Start();
    
			Audio samplesL;  
			Audio samplesR;
			samplesL.SetSize(frameSize);
			samplesR.SetSize(frameSize);

			Audio silence;
			silence.SetSize(frameSize);
								
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
			    _leftChannel.GetAudioChunk(leftIndex,rightIndex,samplesL);
				_rightChannel.GetAudioChunk(leftIndex,rightIndex,samplesR);
				if(!isMutedLChannel())
				{
					channelL.Do(samplesL);
				}
				else
				{
					channelL.Do(silence);
				}
				if(!isMutedRChannel())
				{
					channelR.Do(samplesR);
				}
				else
				{
					channelR.Do(silence);
				}
				leftIndex += frameSize;
				rightIndex += frameSize;
			 }
			 channelL.Stop(); 
			 channelR.Stop();
			 if(!IsPaused()) _time.SetBegin(GetBeginTime());
		}

		void APlayer::SetLeftChannelMuted(bool b)
		{
			_muteLeft=b;
		}

		void APlayer::SetRightChannelMuted(bool b)
		{
			_muteRight=b;
		}

		bool APlayer::isMutedLChannel()
		{
			return _muteLeft;
		}

		bool APlayer::isMutedRChannel()
		{
			return _muteRight;
		}
	}
}

// END

