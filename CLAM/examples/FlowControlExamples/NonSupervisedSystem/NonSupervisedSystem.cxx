#include "NonSupervisedSystem.hxx"

#include <iostream>

#include "AudioIO.hxx"

namespace FlowControlExample
{

System::System( std::string fileIn, std::string fileOut , int frameSize , int nFrames, bool hasAudioOut) : 
	mFileInName(fileIn),
	mFileOutName(fileOut), 
	mFrameSize(frameSize), 
	mMaxFramesToProcess(nFrames),
	mHasAudioOut(hasAudioOut)
{
	ConfigureProcessings();
	ConfigureData();

	StartProcessings();
}

void System::ConfigureProcessings()
{

	// oscillators
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);

	mOscillator.Configure(oscilCfg);

	oscilCfg.SetFrequency(220.0);
	mModulator.Configure(oscilCfg);

	// Audio File In & Out
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( mFileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( mFrameSize );
	fileCfg.SetKeepFrameSizes(true);

	mFileOut.Configure( fileCfg );

	fileCfg.SetFilename(mFileInName);

	mFileIn.Configure (fileCfg);

	if (mHasAudioOut)
	{
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(mFrameSize);

		mAudioOut.Configure(audioCfg);
	}
}

void System::ConfigureData()
{
	mOscillatorData.SetSize(mFrameSize);
	mFileInData.SetSize(mFrameSize);
	mModulatorData.SetSize(mFrameSize);
	mMultiplierData.SetSize(mFrameSize);
	mAdderData.SetSize(mFrameSize);
}

void System::StartProcessings()
{
	try{
		mOscillator.Start();
		mFileOut.Start();
		mFileIn.Start();
		mModulator.Start();
		mMultiplier.Start();
		mAdder.Start();
		if (mHasAudioOut)
		{
			mAudioOut.Start();
		}
	}
	catch (CLAM::ErrProcessingObj& e)
	{
		CLAM_ASSERT(false, e.what());
	}
}

bool System::OscillatorToFileOut() 
{
	mOscillator.Do(mOscillatorData);
	mFileOut.Do(mOscillatorData);
	if (mHasAudioOut)
	{
		mAudioOut.Do(mOscillatorData);
	}

	return false;
}

bool System::ModulatedFileIn()
{
	mFileIn.Do(mFileInData);
	mModulator.Do(mModulatorData);
	mMultiplier.Do(mFileInData, mModulatorData, mMultiplierData);
	mFileOut.Do(mMultiplierData);
	if (mHasAudioOut)
	{
		mAudioOut.Do(mMultiplierData);
	}

	return false;
}

bool System::ModulatedOscillator()
{
	mOscillator.Do(mOscillatorData);
	mModulator.Do(mModulatorData);
	mMultiplier.Do(mOscillatorData, mModulatorData, mMultiplierData);
	mFileOut.Do(mMultiplierData);
	if (mHasAudioOut)
	{
		mAudioOut.Do(mMultiplierData);
	}

	return false;
}

bool System::ModulatedFileInPlusFileIn()
{
	mFileIn.Do(mFileInData);
	mModulator.Do(mModulatorData);
	mMultiplier.Do(mFileInData, mModulatorData, mMultiplierData);
	mAdder.Do(mMultiplierData, mFileInData, mAdderData);
	mFileOut.Do(mAdderData);
	if (mHasAudioOut)
	{
		mAudioOut.Do(mAdderData);
	}

	return false;
}

bool System::FileInFileOut()
{
	mFileIn.Do(mFileInData);
	mFileOut.Do(mFileInData);
	if (mHasAudioOut)
	{
		mAudioOut.Do(mFileInData);
	}
	return false;
}

void System::DoProcessings( IterationMethod iterationDo )
{
	for (int i=0; i<mMaxFramesToProcess; i++)
	{
		if ( (this->*iterationDo)() ) 
		{
			break;
		}
	}
}

void System::ProcessAllIterations()
{
	std::cout << "oscillatortofileout" << std::endl;
	DoProcessings( &System::OscillatorToFileOut );

	std::cout << "modulatedoscillator" << std::endl;
	DoProcessings( &System::ModulatedOscillator );

	std::cout << "fileinfileout" << std::endl;
	DoProcessings( &System::FileInFileOut );

	mFileIn.Stop();
	mFileIn.Start();
	std::cout << "modulatedfilein" << std::endl;
	DoProcessings( &System::ModulatedFileIn );

	mFileIn.Stop();
	mFileIn.Start();
	std::cout << "modulatedfileinplusfilein" << std::endl;
	DoProcessings( &System::ModulatedFileInPlusFileIn);

}

} // namespace
