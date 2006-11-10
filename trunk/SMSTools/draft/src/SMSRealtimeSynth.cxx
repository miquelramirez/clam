#include "SMSRealtimeSynth.hxx"
#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

namespace CLAM
{

int SMSRealtimeSynth::GetSampleRate()
{
	return mSampleRate;
}

void SMSRealtimeSynth::SetSampleRate( int sampleRate )
{
	mSampleRate = sampleRate;
	SetSamplingRate( sampleRate );
}

void SMSRealtimeSynth::LoadAnalysisFile( char* analysisFile )
{
	LoadAnalysis( analysisFile );
	std::cout << "mOriginalSegment.HasBeginTime()" << mOriginalSegment.HasBeginTime() << std::endl;
}

void SMSRealtimeSynth::LoadConfigurationFile( char* configFile )
{
	LoadConfig( configFile );
}

void SMSRealtimeSynth::LoadScoreFile( const std::string& scoreFile )
{
	LoadTransformationScore( scoreFile );
}

void SMSRealtimeSynth::QueryState()
{
	std::cout << "Config: " << GetState().GetHasConfig() << std::endl;
	std::cout << "Analysis: " << GetState().GetHasAnalysis() << std::endl;
	std::cout << "Transformation: " << GetState().GetHasTransformationScore() << std::endl;
}

SMSRealtimeSynth::SMSRealtimeSynth( int sampleRate ) : mSampleRate( sampleRate )
{
}

void SMSRealtimeSynth::Play()
{
	PlayAudio( mAudioOut );
}

void SMSRealtimeSynth::Stream()
{
	ConfigureAudio();

	CLAM::ConnectPorts( GetSynthesis(), "OutputAudio", outL, "Audio Input" );
	CLAM::ConnectPorts( GetSynthesis(), "OutputAudio", outR, "Audio Input" );


	CLAM_ACTIVATE_FAST_ROUNDING;
	
	CopySegmentExceptAudio(mOriginalSegment,mTransformedSegment);	
	int nSynthFrames=mTransformedSegment.GetnFrames();

	TSize synthFrameSize=mSynthConfig.GetFrameSize();
	TIndex beginIndex=-synthFrameSize/2;
	TSize size=synthFrameSize*nSynthFrames;
	mAudioOutSin.SetSize(size);
	mAudioOutRes.SetSize(size);
	mAudioOut.SetSize(size);
	outL.Start();
	outR.Start();

	mTransformation.Configure(mTransformationScore);
	mTransformation.Start();
	GetState().SetHasTransformation(true);
	mTransformedSegment.mCurrentFrameIndex=0;

	GetSynthesis().Configure( mSynthConfig );
	GetSynthesis().Start();

	mTransformedSegment.mCurrentFrameIndex=0;


	for( int i=0; i<nSynthFrames; i++ )
	{
		const Frame& inFrame = mOriginalSegment.GetFramesArray()[i];
		mTransformation.Do( inFrame, mTransformedSegment.GetFramesArray()[i] );
//		printf("synthesis. i==%d\n", i);
		if( !GetSynthesis().Do( mTransformedSegment.GetFramesArray()[i] ) )
		{
			printf("analysis frame with negative center\n");
			continue; // it is an analysis frame with negative center time and thus should not be used
		}

		Frame& currentFrame = mTransformedSegment.GetFramesArray()[i];
//		printf("synthesizing frame at %x\n", &currentFrame);
		Audio& output = currentFrame.GetSynthAudioFrame();
		outL.Do( output );
		outR.Do( output );
		beginIndex+=synthFrameSize;
	}
	
	mTransformation.Stop();
	GetSynthesis().Stop();
	outL.Stop(); 	
	outR.Stop(); 	
	GetState().SetHasAudioOut (true);
	CLAM_DEACTIVATE_FAST_ROUNDING;
	
}

void SMSRealtimeSynth::PlayAudio( const CLAM::Audio& audio ) 
{
				int bufferSize = 1024;
                outL.Start();
                outR.Start();

                CLAM::Audio chunk;

        int offset = 0;

        while ( offset + bufferSize  < audio.GetSize() ) {
                audio.GetAudioChunk( offset, offset + bufferSize, chunk ) ;
                outL.Do(chunk);
                outR.Do(chunk);
                offset += bufferSize;
        }

        outL.Stop();
        outR.Stop();
}

void SMSRealtimeSynth::ConfigureAudio() 
{
//				outLCfg.SetDevice("alsa:default");
                outLCfg.SetChannelID(0);
//              outLCfg.SetSampleRate(mSampleRate);
                outLCfg.SetSampleRate(44100);
                outL.Configure(outLCfg);

                std::cout << "SampleRate for playback is: " << mSampleRate << std::endl;
                
//				outRCfg.SetDevice("alsa:default");
                outRCfg.SetChannelID(1);
//              outRCfg.SetSampleRate(mSampleRate);
                outLCfg.SetSampleRate(44100);
                outR.Configure(outRCfg);
                
                CLAM::AudioManager::Current().Start();
}

} // namespace CLAM

