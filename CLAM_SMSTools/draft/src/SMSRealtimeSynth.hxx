#ifndef SMSREALTIMESYNTH_HXX
#define SMSREALTIMESYNTH_HXX

#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

namespace CLAM
{

class SMSRealtimeSynth : public SMSBase
{
	public:
			SMSRealtimeSynth( int sampleRate );
			void Run() {}
			void Play();
			CLAMGUI::Progress* CreateProgress(const char* title,float from,float to)
			{
				return new CLAMGUI::StdOutProgress(title,from,to);
			}

			CLAMGUI::WaitMessage* CreateWaitMessage(const char* title)
			{
				return new CLAMGUI::StdOutWaitMessage(title);
			}

			int GetSampleRate()
			{
				return mSampleRate;
			}

			void SetSampleRate( int sampleRate )
			{
				mSampleRate = sampleRate;
				SetSamplingRate( sampleRate );
			}

			void LoadAnalysisFile( char* analysisFile )
			{
				LoadAnalysis( analysisFile );
				std::cout << "mOriginalSegment.HasBeginTime()" << mOriginalSegment.HasBeginTime() << std::endl;
			}

			void LoadConfigurationFile( char* configFile )
			{
				LoadConfig( configFile );
			}

			void LoadScoreFile( const std::string& scoreFile )
			{
				LoadTransformationScore( scoreFile );
			}
			void DoTheTransform()
			{
				if(!GetState().GetHasTransformationScore())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available transformation score"<<"\n";
				}
				if(!GetState().GetHasAnalysis())
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
				}

				TransformProcessing();
				Synthesize();
			}

			void QueryState()
			{
				std::cout << "Config: " << GetState().GetHasConfig() << std::endl;
				std::cout << "Analysis: " << GetState().GetHasAnalysis() << std::endl;
				std::cout << "Transformation: " << GetState().GetHasTransformationScore() << std::endl;
			}

	private:
			int mSampleRate;
			CLAM::AudioIOConfig outLCfg;
		  	CLAM::AudioIOConfig outRCfg;
			CLAM::AudioOut outL;
			CLAM::AudioOut outR;

			void PlayAudio( const CLAM::Audio& audio );
			void TransformProcessing();
};

} // namespace CLAM

namespace CLAM
{

SMSRealtimeSynth::SMSRealtimeSynth( int sampleRate ) : mSampleRate( sampleRate )
{
}

void SMSRealtimeSynth::Play()
{
	PlayAudio( mAudioOut );
}

void SMSRealtimeSynth::PlayAudio( const CLAM::Audio& audio ) 
{
				int bufferSize = 256;

//				outLCfg.SetDevice("alsa:default");
                outLCfg.SetChannelID(0);
//                outLCfg.SetSampleRate(mSampleRate);
                outLCfg.SetSampleRate(44100);
                outL.Configure(outLCfg);

                std::cout << "SampleRate for playback is: " << mSampleRate << std::endl;
                
//				outRCfg.SetDevice("alsa:default");
                outRCfg.SetChannelID(1);
//                outRCfg.SetSampleRate(mSampleRate);
                outLCfg.SetSampleRate(44100);
                outR.Configure(outRCfg);
                


                CLAM::AudioManager::Current().Start();

                outL.Start();
                outR.Start();

                CLAM::Audio chunk;

				std::cout << "mAudioOut.size(): " << mAudioOut.GetSize() << std::endl;
				std::cout << "bufferSize: " << bufferSize << std::endl;

        int offset = 0;

        while ( offset + bufferSize  < audio.GetSize() ) {
                audio.GetAudioChunk( offset, offset+bufferSize, chunk ) ;
                outL.Do(chunk);
                outR.Do(chunk);
                offset += bufferSize;
        }

        outL.Stop();
        outR.Stop();
}

void SMSRealtimeSynth::TransformProcessing() 
{
	CLAM_ACTIVATE_FAST_ROUNDING;
	//UpdateDataInTimeStretch();
	mTransformation.Configure(mTransformationScore);
	//ConfigureSMSMorph();
	CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);	
	

	mTransformation.Start();

	//int i = 0;
	while(mTransformation.Do())
	{
	//	CLAM_ASSERT( mCurrentProgressIndicator, 
	//			"SMSBase::TransformProcessing mCurrentProgressIndicator should't be NULL"
	//			" Probably you din't call Transfrom()" );
	//	mCurrentProgressIndicator->Update(float(i++));
	}
	mTransformation.Stop();
	GetState().SetHasTransformation(true);
	CLAM_DEACTIVATE_FAST_ROUNDING;

}

} // namespace CLAM

#endif
