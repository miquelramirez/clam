#include "NonSupervisedSystemWithPorts.hxx"

#include "AudioIO.hxx"

#include <iostream>

namespace FlowControlExample
{

bool NetworkConfiguration::trace = true;

void NetworkConfiguration::ConnectAndDo()
{
	if (trace) std::cout << "Processing: " << GetName();
	Start();
	Connect(); 
	if (trace) std::cout << "  Connected.  ";
	for (int i=0; i<mSystem->GetMaxFramesToProcess(); i++) {
		if ( Do() ) break;
	}
	if (trace) std::cout <<"Done.\n";
	Stop();
}


SystemWithPorts::SystemWithPorts( std::string fileIn, std::string fileOut , int frameSize , int nFrames, bool hasAudioOut ) :
	mAudioManager(44100, frameSize),
	mControlSender(20,44100,0,frameSize),
	mFileInName(fileIn),
	mFileOutName(fileOut), 
	mFrameSize(frameSize), 
	mMaxFramesToProcess(nFrames),
	mHasAudioOut(hasAudioOut)

{
	AddNetworkConfiguration( new OscillatorToFileOut( this ) );
	AddNetworkConfiguration( new FileInFileOut( this ) );
	AddNetworkConfiguration( new ModulatedFileIn( this ) );
	AddNetworkConfiguration( new ModulatedOscillator( this ) );
	AddNetworkConfiguration( new ModulatedFileInPlusFileIn( this ) );


	RegisterProcessings();
	ConfigureProcessings();
	ConfigureData();
	StartProcessings();


}
SystemWithPorts::~SystemWithPorts()
{
	RemoveAllNetworkConfigurations();
}

void SystemWithPorts::RegisterProcessings()
{
	mProcessings.push_back( &mOscillator );
	mProcessings.push_back( &mModulator );
	mProcessings.push_back( &mFileIn );
	mProcessings.push_back( &mFileOut );
	mProcessings.push_back( &mMultiplier );
	mProcessings.push_back( &mAudioOut);
	mProcessings.push_back( &mMixer );
//	mProcessings.push_back( &mControlSender );
	
}

void SystemWithPorts::ConfigureProcessings()
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

	CLAM::AudioMixerConfig mixerCfg;
	mixerCfg.SetFrameSize(mFrameSize);

	mMixer.Configure( mixerCfg );

	if (mHasAudioOut)
	{
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(mFrameSize);

		mAudioOut.Configure(audioCfg);
	}


}

void SystemWithPorts::ConfigureData()
{
	mOscillatorData.SetSize(mFrameSize);
	mFileInData.SetSize(mFrameSize);
	mModulatorData.SetSize(mFrameSize);
	mMultiplierData.SetSize(mFrameSize);
	mMixerData.SetSize(mFrameSize);
}

void SystemWithPorts::StartProcessings()
{
	try{
		mOscillator.Start();
		mFileOut.Start();
		mFileIn.Start();
		mModulator.Start();
		mMultiplier.Start();
		mMixer.Start();

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

void SystemWithPorts::OscillatorToFileOut::Connect()
{
	System().mOscillator.GetOutPorts().Get( "Audio Output" ).Attach( System().mOscillatorData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( System().mOscillatorData );
	System().AudioOutAttach(System().mOscillatorData);

}
bool SystemWithPorts::OscillatorToFileOut::Do() 
{
	System().mOscillator.Do();
	System().mFileOut.Do();
	System().AudioOutDo();
	return false;
}

void SystemWithPorts::ModulatedFileIn::Connect()
{
	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( System().mFileInData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( System().mFileInData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( System().mMultiplierData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( System().mMultiplierData );
	System().AudioOutAttach(System().mMultiplierData);
}
bool SystemWithPorts::ModulatedFileIn::Do()
{
	System().mFileIn.Do();
	System().mModulator.Do();
	System().mMultiplier.Do();
	System().mFileOut.Do();
	System().AudioOutDo();	

	return false;
}
void SystemWithPorts::ModulatedFileIn::Stop()
{
	System().mFileIn.Stop();
	System().mFileIn.Start();
}

void SystemWithPorts::ModulatedOscillator::Connect()
{
	System().mOscillator.GetOutPorts().Get( "Audio Output" ).Attach( System().mOscillatorData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( System().mOscillatorData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( System().mMultiplierData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( System().mMultiplierData );
	System().AudioOutAttach(System().mMultiplierData);
}
bool SystemWithPorts::ModulatedOscillator::Do()
{
	System().mOscillator.Do();
	System().mModulator.Do();
	System().mMultiplier.Do();
	System().mFileOut.Do();
	System().AudioOutDo();
	System().AudioOutDo();

	return false;
}

void SystemWithPorts::FileInFileOut::Connect()
{

	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( System().mFileInData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( System().mFileInData );
	System().AudioOutAttach(System().mFileInData);
}
bool SystemWithPorts::FileInFileOut::Do()
{
	System().mFileIn.Do();
	System().mFileOut.Do();
	System().AudioOutDo();

	return false;
}
void SystemWithPorts::FileInFileOut::Stop()
{
	System().mFileIn.Stop();
	System().mFileIn.Start();
}

void SystemWithPorts::ModulatedFileInPlusFileIn::Connect()
{
	// linking ControlSender with AudioMixer volumes.
	System().mControlSender.mLeft.AddLink(&(System().mMixer.GetInControls().Get("Input Gain_0")));
	System().mControlSender.mRight.AddLink(&(System().mMixer.GetInControls().Get("Input Gain_1")));

	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( System().mFileInData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( System().mFileInData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( System().mMultiplierData );
	System().mMixer.GetInPorts().Get("Input Audio_0").Attach( System().mMultiplierData );
	System().mMixer.GetInPorts().Get("Input Audio_1").Attach( System().mFileInData );
	System().mMixer.GetOutPorts().Get("Output Audio").Attach( System().mMixerData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( System().mMixerData );
	System().AudioOutAttach(System().mMixerData);
}
bool SystemWithPorts::ModulatedFileInPlusFileIn::Do()
{
	System().mFileIn.Do();
	System().mModulator.Do();
	System().mMultiplier.Do();
	System().mControlSender.Do();
	System().mMixer.Do();
	System().mFileOut.Do();
	System().AudioOutDo();
	return false;
}
void SystemWithPorts::ModulatedFileInPlusFileIn::Stop() {
	System().mFileIn.Stop();
	System().mFileIn.Start();
}


void SystemWithPorts::AddNetworkConfiguration( NetworkConfiguration* item)
{
	mNetworks.push_back( item );
}
void SystemWithPorts::RemoveAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=mNetworks.begin(); it!=mNetworks.end(); it++)
		delete (*it);
}
void SystemWithPorts::ProcessAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=mNetworks.begin(); it!=mNetworks.end(); it++) {
		(*it)->ConnectAndDo();
	}
}


bool SystemWithPorts::AudioOutDo()
{
	if (mHasAudioOut)
	{
		mAudioOut.Do();
		return true;
	}
	return false;
}

bool SystemWithPorts::AudioOutAttach(CLAM::Audio& a)
{
	if (mHasAudioOut)
	{
		mAudioOut.GetInPorts().Get( "Input" ).Attach( a );
		return true;
	}
	return false;
}


} // namespace
