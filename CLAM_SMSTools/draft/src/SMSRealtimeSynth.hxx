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
				//Synthesize();
			}

			void QueryState()
			{
				std::cout << "Config: " << GetState().GetHasConfig() << std::endl;
				std::cout << "Analysis: " << GetState().GetHasAnalysis() << std::endl;
				std::cout << "Transformation: " << GetState().GetHasTransformationScore() << std::endl;
			}

	private:
			int mSampleRate;
			int mBufferSize;
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
        try {

//				outLCfg.SetDevice("alsa:default");
//                outLCfg.SetChannelID(0);
//                outLCfg.SetSampleRate(mSampleRate);
//                outL.Configure(outLCfg);
//
//                std::cout << "SampleRate for playback is: " << mSampleRate << std::endl;
//                
//				outRCfg.SetDevice("alsa:default");
//                outRCfg.SetChannelID(1);
//                outRCfg.SetSampleRate(mSampleRate);
//                outR.Configure(outRCfg);
//                
//

                CLAM::AudioManager::Current().Start();

                outL.Start();
                outR.Start();

                CLAM::Audio chunk;

        int offset = 0;

        while ( offset  < audio.GetSize() ) {
                audio.GetAudioChunk( offset, offset+mBufferSize, chunk ) ;
                outL.Do(chunk);
                outR.Do(chunk);
                offset += mBufferSize;
        }

                outL.Stop();
                outR.Stop();
                

        } catch(CLAM::Err error) {
                error.Print();
                std::cerr << "Abnormal Program Termination" << std::endl;
                exit(-1);
         }
         catch (std::exception e) {
                std::cout << e.what() << std::endl;
                exit(-1);
        }
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
