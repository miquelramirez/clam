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

void SMSRealtimeSynth::DoTheTransform()
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

	CLAM::ConnectPorts( mSynthesis, "OutputAudio", outL, "Audio Input" );
	CLAM::ConnectPorts( mSynthesis, "OutputAudio", outR, "Audio Input" );

	DoSynthesis();
}

void SMSRealtimeSynth::DoSynthesis()
{
	SynthesisProcessing();
}

void SMSRealtimeSynth::SynthesisProcessing()
{
	mSynthesis.Configure( mSynthConfig );
	mSynthesis.Start();

	CLAM_ACTIVATE_FAST_ROUNDING;
	GetSynthesis().Start();
	
	Audio tmpAudioFrame,tmpAudioFrame2;
	tmpAudioFrame.SetSize(mSynthConfig.GetFrameSize());
		
	int nSynthFrames=mTransformedSegment.GetnFrames();


	TSize synthFrameSize=mSynthConfig.GetFrameSize();
	TIndex beginIndex=-synthFrameSize/2;
	
	TSize size=synthFrameSize*nSynthFrames;

	mAudioOutSin.SetSize(size);
	mAudioOutRes.SetSize(size);
	mAudioOut.SetSize(size);


	mTransformedSegment.mCurrentFrameIndex=0;

	outL.Start();
	outR.Start();

	for( int i=0; i<nSynthFrames; i++ )
	{
		
		if( !GetSynthesis().Do(mTransformedSegment) )
			continue; // it is an analysis frame with negative center time and thus should not be used

//		output = mTransformedSegment.GetFramesArray()[i].GetSinusoidalAudioFrame();
//		output = mTransformedSegment.GetFramesArray()[i].GetResidualAudioFrame();
		Audio& output = mTransformedSegment.GetFramesArray()[i].GetSynthAudioFrame();
		outL.Do( output );
		outR.Do( output );
		//mAudioOutSin.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSinusoidalAudioFrame());
		//mAudioOutRes.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetResidualAudioFrame());
		//mAudioOut.SetAudioChunk(beginIndex,mTransformedSegment.GetFramesArray()[i].GetSynthAudioFrame());
		beginIndex+=synthFrameSize;
	}
	
	mSynthesis.Stop();
	outL.Stop(); 	
	outR.Stop(); 	
	GetState().SetHasAudioOut (true);
	GetSynthesis().Stop();
	CLAM_DEACTIVATE_FAST_ROUNDING;
	
}

void SMSRealtimeSynth::PlayAudio( const CLAM::Audio& audio ) 
{
				int bufferSize = 512;
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

