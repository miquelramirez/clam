#include "Segment.hxx"
#include "AudioIO.hxx" 
#include "AudioManager.hxx" 
#include "AudioOut.hxx" 
#include "SimpleOscillator.hxx" 
#include "vmFundFreqPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		FundFreqPlayer::FundFreqPlayer(QObject* parent)
			: Player(parent)
			, mSegment(0)
		{
			SetPlayingFlags(CLAM::VM::eUseOscillator);
		}

		FundFreqPlayer::~FundFreqPlayer()
		{
		}

		void FundFreqPlayer::SetData(const Segment& segment)
		{
			mSegment = &segment;
		}

		void FundFreqPlayer::run()
		{
			if(!mSegment) return;
			if(!mSegment->GetnFrames()) return;
			if(!mSegment->HasAudio()) mPlayingFlags = CLAM::VM::eUseOscillator;

			mSamplingRate = mSegment->GetSamplingRate(); 
			unsigned nSamples = unsigned((mSegment->GetEndTime()-mSegment->GetBeginTime())*mSamplingRate);
			int frameSize = 512;                    

			AudioManager manager((int)mSamplingRate,frameSize);  
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
			oscCfg.SetSamplingRate(mSamplingRate);
			oscCfg.SetAmplitude(0.6);
			SimpleOscillator osc(oscCfg);
			
			InControl& freqControl = osc.GetInControls().Get("Pitch");

			int nFrames = mSegment->GetnFrames();

			osc.Start();

			unsigned leftIndex = unsigned(mBeginTime*mSamplingRate);
			unsigned rightIndex = leftIndex+frameSize;

			while(leftIndex < unsigned(mTimeBounds.max*mSamplingRate))
			{
				if(!mIsPlaying) break;
				if(mPlayingFlags & CLAM::VM::eAudio)
				{
					mSegment->GetAudio().GetAudioChunk(int(leftIndex),int(rightIndex),samples0);
				}
				freqControl.DoControl(mSegment->GetFrame(int(leftIndex*nFrames/nSamples)).GetFundamental().GetFreq(0));
				osc.Do(samples1);		
				if((mPlayingFlags & CLAM::VM::eAudio) && (mPlayingFlags & CLAM::VM::eUseOscillator))
				{
					channel0.Do(samples0);
					channel1.Do(samples1);
				}
				else if(mPlayingFlags & CLAM::VM::eAudio)
				{
					channel0.Do(samples0);
					channel1.Do(samples0);
				}
				else
				{
					channel0.Do(samples1);
					channel1.Do(samples1);
				}			   
				emit playingTime(double(leftIndex)/mSamplingRate);
				leftIndex += frameSize;
				rightIndex += frameSize;
			}
			osc.Stop();
			channel0.Stop();
			channel1.Stop();

			(mIsPaused) ? mBeginTime = double(leftIndex)/mSamplingRate : mBeginTime = mTimeBounds.min;
			emit stopTime(double(leftIndex)/mSamplingRate,mIsPaused);
			mIsPlaying = false;
		}
	}
}

// END
