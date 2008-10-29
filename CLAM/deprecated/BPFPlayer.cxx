#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "SimpleOscillator.hxx" 
#include "BPFPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFPlayer::BPFPlayer()
			: mAudioPtrL(0)
			, mAudioPtrR(0)
			, mSampleRate(TData(44100.0))
		{
			HaveData(true);
			SetDuration(TData(0.0));
			mThread.SetThreadCode(makeMemberFunctor0((*this), BPFPlayer, thread_code));
		}

		BPFPlayer::~BPFPlayer()
		{
		}

		void BPFPlayer::SetData(const BPF& bpf)
		{
			mBPFData = bpf;
		}

		void BPFPlayer::SetAudioPtr(const Audio* audio, int chn)
		{
			switch(chn)
			{
				case CLAM::VM::LEFT_CHANNEL:
					mAudioPtrL=audio;
					break;
				case CLAM::VM::RIGHT_CHANNEL:
					mAudioPtrR=audio;
					break;
				case CLAM::VM::BOTH_CHANNELS:
					mAudioPtrL=audio;
					mAudioPtrR=audio;
					break;
				default:
					mAudioPtrL=0;
					mAudioPtrR=0;
					break;
			}
		}

		void BPFPlayer::SetDuration(TData duration)
		{
			mTime.SetBegin(TData(0.0));
			mTime.SetEnd(duration);
		}

		void BPFPlayer::SetSampleRate(TData sr)
		{
			mSampleRate = sr;
		}

		void BPFPlayer::Update(TIndex index, TData yvalue)
		{
			if(index < 0 || index >= mBPFData.Size()) return;
			mBPFData.SetValue(index,yvalue);
		}

		void BPFPlayer::thread_code()
		{
			if(!mBPFData.Size() && !mAudioPtrL && !mAudioPtrR) return;

			TSize frameSize = 512;     
			AudioManager manager((int)mSampleRate,(int)frameSize);  

			AudioOut channelL;   
			AudioIOConfig audioOutCfgL;     
			audioOutCfgL.SetChannelID(0);    
			channelL.Configure(audioOutCfgL); 

			AudioOut channelR;   
			AudioIOConfig audioOutCfgR;     
			audioOutCfgR.SetChannelID(1);    
			channelR.Configure(audioOutCfgR); 
			
			
			manager.Start();                            
			channelL.Start();  
			channelR.Start();

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(mSampleRate);
			oscCfg.SetAmplitude(TData(0.6));
			SimpleOscillator osc(oscCfg);
			
			InControlBase& freqControl = osc.GetInControls().Get("Pitch");
    
			Audio samplesL;             
			Audio samplesR;
			samplesL.SetSize(frameSize);
			samplesR.SetSize(frameSize);

			TIndex firstIndex = GetFirstIndex();
			TIndex k = firstIndex;

			TIndex start = int(mTime.GetBegin()*mSampleRate);
			int nSamples = int(mTime.GetEnd()*mSampleRate);
	    
			TIndex leftIndex = start;        
			TIndex rightIndex = leftIndex+frameSize;

			osc.Start();

			for(TIndex i=start; i < nSamples; i+=frameSize)
			{
				if(IsPaused())
				{
					mTime.SetBegin(TData(i)/mSampleRate);
					SetPlaying(false);
				}

				if(!IsPlaying()) break;

				if(k < mBPFData.Size()-1) if(TData(i/mSampleRate) >= mBPFData.GetXValue(k+1)) k++;

				if (k<mBPFData.Size())
					freqControl.DoControl(mBPFData.GetValueFromIndex(k));
				
				if(mAudioPtrL) 
				{
					if(rightIndex < mAudioPtrL->GetSize())
					{
						mAudioPtrL->GetAudioChunk(leftIndex,rightIndex,samplesL);
					}
				}
				else
				{
					osc.Do(samplesL);
				}
				if(mAudioPtrR) 
				{
					if(rightIndex < mAudioPtrR->GetSize())
					{
						mAudioPtrR->GetAudioChunk(leftIndex,rightIndex,samplesR);
					}
				}
				if(mAudioPtrL || mAudioPtrR)
				{
					channelL.Do(samplesL);
					channelR.Do(samplesR);
				}
				else
				{
					channelL.Do(samplesL);
					channelR.Do(samplesL);
				}

				mSigPlayingTime.Emit(TData(leftIndex)/mSampleRate);

				leftIndex += frameSize;
				rightIndex += frameSize;
			}

			osc.Stop();
			channelL.Stop(); 
			channelR.Stop();

			if(!IsPaused()) mTime.SetBegin(GetBeginTime());
			TData stopTime = 
				(IsStopped()) ? TData(leftIndex)/mSampleRate : 
				(IsPaused()) ? mTime.GetBegin() : mTime.GetEnd();

			mSigStop.Emit(stopTime);
		}

		TIndex BPFPlayer::GetFirstIndex()
		{
			int nPoints = mBPFData.Size();
			if(nPoints <= 1) return 0;

			TData searchValue = mTime.GetBegin();
			if(searchValue <= mBPFData.GetXValue(0)) return 0;
			if(searchValue >= mBPFData.GetXValue(nPoints-1)) return nPoints-1;

			TIndex index = 0;
			TIndex left_index = 0;
			TIndex right_index = nPoints-1;
			while(left_index <= right_index)
			{
				const TIndex currentIndex = (left_index+right_index)/2;
				if(currentIndex >= nPoints-1)
				{
					index = currentIndex;
					break;
				}
				if(searchValue >= mBPFData.GetXValue(currentIndex) && 
				   searchValue <= mBPFData.GetXValue(currentIndex+1))
				{
					index = currentIndex;
					break;
				}
				if(searchValue < mBPFData.GetXValue(currentIndex))
				{
					right_index = currentIndex-1;
				}
				else if(searchValue > mBPFData.GetXValue(currentIndex))
				{
					left_index = currentIndex+1;
				}
			}
			return index;
		}
	}
}

// END

