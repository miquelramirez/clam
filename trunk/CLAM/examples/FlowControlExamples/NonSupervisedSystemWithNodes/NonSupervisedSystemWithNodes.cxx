
#include "NonSupervisedSystemWithNodes.hxx"

#include "AudioIO.hxx"

#include <iostream>

namespace FlowControlExample
{

bool NetworkConfiguration::trace = true;

//! Template method. The concrete behaviour is in Start(), Connect(), Do(), Stop()
void NetworkConfiguration::ConnectAndDo()
{
	if (trace) std::cout << "Processing: " << GetName();
	System().ResetAllNodes();
	Start();
	Connect(); 
	if (trace) std::cout << "  Connected.  ";
	for (int i=0; i<mSystem->GetMaxFramesToProcess(); i++) {
		if ( Do() ) break;
	}
	if (trace) std::cout <<"Done.\n";
	Stop();
}


SystemWithNodes::SystemWithNodes( std::string fileIn, std::string fileOut , int frameSize , int nFrames, bool hasAudioOut ) :
	mAudioManager(44100, frameSize),
	mFileInName(fileIn),
	mFileOutName(fileOut), 
	mFrameSize(frameSize), 
	mMaxFramesToProcess(nFrames),
	mHasAudioOut(hasAudioOut),
	mOscillatorData(0),
	mFileInData(0),
	mModulatorData(0),
	mMixerData(0),
	mMultiplierData(0)

{
	AddNetworkConfiguration( new OscillatorToFileOut( this ) );
	AddNetworkConfiguration( new FileInFileOut( this ) );
	AddNetworkConfiguration( new ModulatedFileIn( this ) );
	AddNetworkConfiguration( new ModulatedOscillator( this ) );
	AddNetworkConfiguration( new ModulatedFileInPlusFileIn( this ) );


	RegisterProcessings();
	ConfigureProcessings();
	ConfigurePorts();
	StartProcessings();
}

SystemWithNodes::~SystemWithNodes()
{
	RemoveAllNetworkConfigurations();
	DeleteAllNodes();
}

void SystemWithNodes::RegisterProcessings()
{
	mProcessings.push_back( &mOscillator );
	mProcessings.push_back( &mModulator );
	mProcessings.push_back( &mFileIn );
	mProcessings.push_back( &mFileOut );
	mProcessings.push_back( &mMultiplier );
	mProcessings.push_back( &mAudioOut);
	mProcessings.push_back( &mMixer );
	mProcessings.push_back( &mControlSender );
	
}

void SystemWithNodes::ConfigureProcessings()
{

	// oscillators
	CLAM::SimpleOscillatorConfig oscilCfg;
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

	CLAM::AutoPannerConfig pannerCfg;
	pannerCfg.SetFrequency( 20.0 );
	pannerCfg.SetFrameSize(mFrameSize);
	pannerCfg.SetSamplingRate(44100);
	mControlSender.Configure( pannerCfg );


	if (mHasAudioOut)
	{
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(mFrameSize);

		mAudioOut.Configure(audioCfg);
	}
}

void SystemWithNodes::StartProcessings()
{
	try{
		mOscillator.Start();
		mFileOut.Start();
		mFileIn.Start();
		mModulator.Start();
		mMultiplier.Start();
		mMixer.Start();
		mControlSender.Start();

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

void SystemWithNodes::OscillatorToFileOut::Connect()
{
	//attaching ports to node
	System().mOscillator.GetOutPorts().Get( "Audio Output" ).Attach( *System().mOscillatorData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( *System().mOscillatorData );
	System().ConditionalAudioOutAttach( *System().mOscillatorData );

	// configuring node with frame size
	System().mOscillatorData->Configure( System().mFrameSize );
}

bool SystemWithNodes::OscillatorToFileOut::Do() 
{
	System().mOscillator.Do();
	System().mFileOut.Do();
	System().AudioOutDo();
	return false;
}

void SystemWithNodes::ModulatedFileIn::Connect()
{
	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( *System().mFileInData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System().mFileInData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System().mMultiplierData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( *System().mMultiplierData );
	System().ConditionalAudioOutAttach( *System().mMultiplierData );

	System().mFileInData->Configure( System().mFrameSize );
	System().mModulatorData->Configure( System().mFrameSize );
	System().mMultiplierData->Configure( System().mFrameSize );
	
}
bool SystemWithNodes::ModulatedFileIn::Do()
{
	System().mFileIn.Do();
	System().mModulator.Do();
	System().mMultiplier.Do();
	System().mFileOut.Do();
	System().AudioOutDo();	

	return false;
}
void SystemWithNodes::ModulatedFileIn::Stop()
{
	System().mFileIn.Stop();
	System().mFileIn.Start();
}

void SystemWithNodes::ModulatedOscillator::Connect()
{
	System().mOscillator.GetOutPorts().Get( "Audio Output" ).Attach( *System().mOscillatorData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System().mOscillatorData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System().mMultiplierData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( *System().mMultiplierData );
	System().ConditionalAudioOutAttach( *System().mMultiplierData );


	System().mOscillatorData->Configure( System().mFrameSize );
	System().mModulatorData->Configure( System().mFrameSize );
	System().mMultiplierData->Configure( System().mFrameSize );

}
bool SystemWithNodes::ModulatedOscillator::Do()
{
	System().mOscillator.Do();
	System().mModulator.Do();
	System().mMultiplier.Do();
	System().mFileOut.Do();
	System().AudioOutDo();
	System().AudioOutDo();

	return false;
}

void SystemWithNodes::FileInFileOut::Connect()
{

	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( *System().mFileInData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( *System().mFileInData );
	System().ConditionalAudioOutAttach( *System().mFileInData );

	System().mFileInData->Configure( System().mFrameSize );

}
bool SystemWithNodes::FileInFileOut::Do()
{
	System().mFileIn.Do();
	System().mFileOut.Do();
	System().AudioOutDo();

	return false;
}
void SystemWithNodes::FileInFileOut::Stop()
{
	System().mFileIn.Stop();
	System().mFileIn.Start();
}

void SystemWithNodes::ModulatedFileInPlusFileIn::Connect()
{
	// linking ControlSender with AudioMixer volumes.
	System().mControlSender.GetOutControls().Get( "Left Control").AddLink(&(System().mMixer.GetInControls().Get("Input Gain_0")));
	System().mControlSender.GetOutControls().Get( "Right Control").AddLink(&(System().mMixer.GetInControls().Get("Input Gain_1")));

	// attach for each Port
	System().mFileIn.GetOutPorts().Get( "Output" ).Attach( *System().mFileInData );
	System().mModulator.GetOutPorts().Get( "Audio Output" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System().mFileInData );
	System().mMultiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System().mModulatorData );
	System().mMultiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System().mMultiplierData );
	System().mMixer.GetInPorts().Get("Input Audio_0").Attach( *System().mMultiplierData );
	System().mMixer.GetInPorts().Get("Input Audio_1").Attach( *System().mFileInData );
	System().mMixer.GetOutPorts().Get("Output Audio").Attach( *System().mMixerData );
	System().mFileOut.GetInPorts().Get( "Input" ).Attach( *System().mMixerData );
	System().ConditionalAudioOutAttach( *System().mMixerData);

	System().mFileInData->Configure( System().mFrameSize );
	System().mModulatorData->Configure( System().mFrameSize );
	System().mMultiplierData->Configure( System().mFrameSize );
	System().mMixerData->Configure( System().mFrameSize );
}
bool SystemWithNodes::ModulatedFileInPlusFileIn::Do()
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
void SystemWithNodes::ModulatedFileInPlusFileIn::Stop() {
	System().mFileIn.Stop();
	System().mFileIn.Start();
}


void SystemWithNodes::AddNetworkConfiguration( NetworkConfiguration* item)
{
	mNetworks.push_back( item );
}
void SystemWithNodes::RemoveAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=mNetworks.begin(); it!=mNetworks.end(); it++)
		delete (*it);
}
void SystemWithNodes::ProcessAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=mNetworks.begin(); it!=mNetworks.end(); it++) {
		(*it)->ConnectAndDo();
	}
}


bool SystemWithNodes::AudioOutDo()
{
	if (mHasAudioOut)
	{
		mAudioOut.Do();
		return true;
	}
	return false;
}

bool SystemWithNodes::ConditionalAudioOutAttach( AudioNode& a)
{
	if (mHasAudioOut)
	{
		mAudioOut.GetInPorts().Get( "Input" ).Attach( a );
		return true;
	}
	return false;
}

void SystemWithNodes::UnattachAllPorts()
{	
	mFileIn.GetOutPorts().Get( "Output" ).Unattach();
	mModulator.GetOutPorts().Get( "Audio Output" ).Unattach();
	mOscillator.GetOutPorts().Get( "Audio Output" ).Unattach();
//	mMultiplier.GetInPorts().Get( "First Audio Input" ).Unattach();
//	mMultiplier.GetInPorts().Get( "Second Audio Input" ).Unattach();
	mMultiplier.GetOutPorts().Get( "Audio Output" ).Unattach();
//	mMixer.GetInPorts().Get("Input Audio_0").Unattach();
//	mMixer.GetInPorts().Get("Input Audio_1").Unattach();
	mMixer.GetOutPorts().Get("Output Audio").Unattach();
//	mFileOut.GetInPorts().Get( "Input" ).Unattach();
//	mAudioOut.GetInPorts().Get( "Input" ).Unattach();

}

void SystemWithNodes::DeleteAllNodes()
{
//	if ( mOscillatorData ) delete mOscillatorData;
//	if ( mFileInData ) delete mFileInData;
//	if ( mModulatorData ) delete mModulatorData;
//	if ( mMixerData ) delete mMixerData;
//	if ( mMultiplierData ) delete mMultiplierData;
}
void SystemWithNodes::ResetAllNodes()
{
	// this is a provisional way of reset nodes! 
	// Nodes should have a Reset method
	// todo: change this method when new Node interface is ready
	UnattachAllPorts();
//	DeleteAllNodes();

	mOscillatorData = new AudioNode();
	mFileInData = new AudioNode();
	mModulatorData = new AudioNode();
	mMixerData = new AudioNode();
	mMultiplierData = new AudioNode();
}

void SystemWithNodes::ConfigurePorts()
{
	mOscillator.GetOutPorts().Get( "Audio Output" ).SetParams( mFrameSize );
	mFileIn.GetOutPorts().Get( "Output" ).SetParams( mFrameSize );
	mModulator.GetOutPorts().Get( "Audio Output" ).SetParams( mFrameSize );
	mMultiplier.GetInPorts().Get( "First Audio Input" ).SetParams( mFrameSize );
	mMultiplier.GetInPorts().Get( "Second Audio Input" ).SetParams( mFrameSize );
	mMultiplier.GetOutPorts().Get( "Audio Output" ).SetParams( mFrameSize );
	mMixer.GetInPorts().Get("Input Audio_0").SetParams( mFrameSize );
	mMixer.GetInPorts().Get("Input Audio_1").SetParams( mFrameSize );
	mMixer.GetOutPorts().Get("Output Audio").SetParams( mFrameSize );
	mFileOut.GetInPorts().Get( "Input" ).SetParams( mFrameSize );
	if ( mHasAudioOut )
		mAudioOut.GetInPorts().Get( "Input" ).SetParams( mFrameSize );
}


} // namespace
