
#include "SupervisedSystemWithoutTrueFlowControl.hxx"


#include "AudioMultiplier.hxx"
#include "PushFlowControl.hxx"
#include "BasicFlowControl.hxx"

namespace FlowControlExample
{

// constructor
SupervisedSystemWithoutTrueFlowControl::SupervisedSystemWithoutTrueFlowControl ( 
	std::string fileIn, 
	std::string fileOut , 
	int frameSize, 
	int maxFramesToProcess, 
	bool hasAudioOut )	:

	mAudioManager(44100, frameSize),
	mFrameSize(frameSize), 
	mMaxFramesToProcess(maxFramesToProcess),
	mHasAudioOut(hasAudioOut),
	mFileInName(fileIn),
	mFileOutName(fileOut)
{
	mNetwork.AddFlowControl( new CLAM::PushFlowControl( mFrameSize ));
	ConfigureAndAddProcessingsToNetwork();
	RegisterAllNetworkConfigurationMethods();
}


void SupervisedSystemWithoutTrueFlowControl::ConfigureAndAddProcessingsToNetwork()
{
	mNetwork.AddProcessing("file-in", new CLAM::AudioFileIn( CreateFileInCfg() ));
	mNetwork.AddProcessing("oscillator-generator", new CLAM::Oscillator( CreateGeneratorCfg() ) );
	mNetwork.AddProcessing("oscillator-modulator", new CLAM::Oscillator( CreateModulatorCfg() ) );
	mNetwork.AddProcessing("multiplier", new CLAM::AudioMultiplier );
	mNetwork.AddProcessing( "mixer", new CLAM::AudioMixer<2>( CreateMixerCfg() ) );
	mNetwork.AddProcessing("file-out", new CLAM::AudioFileOut( CreateFileOutCfg() ) );
	//mNetwork.AddProcessing("audio-out", new CLAM::AudioOut( CreateAudioOutCfg() ) );
}

CLAM::AudioFileConfig SupervisedSystemWithoutTrueFlowControl::CreateFileInCfg()
{
	CLAM::AudioFileConfig cfg;
	cfg.SetChannels(1);
	cfg.SetFiletype( CLAM::EAudioFileType::eWave );
	cfg.SetFrameSize( mFrameSize );
	cfg.SetFilename( mFileInName );
	cfg.SetKeepFrameSizes(true);
	return cfg;
}
CLAM::OscillatorConfig SupervisedSystemWithoutTrueFlowControl::CreateGeneratorCfg()
{
	CLAM::OscillatorConfig cfg;
	cfg.SetFrequency(440.0);
	cfg.SetAmplitude(0.5);
	return cfg;
}
CLAM::OscillatorConfig SupervisedSystemWithoutTrueFlowControl::CreateModulatorCfg()
{
	CLAM::OscillatorConfig cfg;
	cfg.SetFrequency(220.0);
	cfg.SetAmplitude(0.5);
	return cfg;
}
CLAM::AudioMixerConfig SupervisedSystemWithoutTrueFlowControl::CreateMixerCfg()
{	
	CLAM::AudioMixerConfig cfg;
	cfg.SetFrameSize(mFrameSize);
	return cfg;
}
CLAM::AudioFileConfig SupervisedSystemWithoutTrueFlowControl::CreateFileOutCfg()
{
	CLAM::AudioFileConfig cfg;
	cfg.SetChannels(1);
	cfg.SetFiletype( CLAM::EAudioFileType::eWave );
	cfg.SetFrameSize( mFrameSize );
	cfg.SetFilename( mFileOutName );
	cfg.SetKeepFrameSizes(true);
	return cfg;
}
CLAM::AudioIOConfig SupervisedSystemWithoutTrueFlowControl::CreateAudioOutCfg()
{
	CLAM::AudioIOConfig cfg;
	cfg.SetFrameSize(mFrameSize);
	return cfg;
}
//---------------------------------------------------------------------------------


void SupervisedSystemWithoutTrueFlowControl::RegisterAllNetworkConfigurationMethods()
{

	mConfigurations.push_back( NetworkConfigurationMethodWrapper(
		this, &SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut ) );
	mConfigurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator ) );
	mConfigurations.push_back( 	NetworkConfigurationMethodWrapper(
		this, &SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut ) );
	mConfigurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn ) );
	mConfigurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn ) );

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut()
{
	mNetwork.SetName("Oscillator to FileOut");
	mNetwork.ConnectPorts( "oscillator-generator.Audio Output", "file-out.Input" );

	if (mHasAudioOut)
		mNetwork.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
	
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{
	mNetwork.SetName("FileIn to FileOut");
	mNetwork.ConnectPorts( "file-in.Output", "file-out.Input" );
	
	if (mHasAudioOut)
		mNetwork.ConnectPorts( "file-in.Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{
	mNetwork.SetName("Modulated FileIn");
	mNetwork.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	mNetwork.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	mNetwork.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (mHasAudioOut)
		mNetwork.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
	mNetwork.SetName("Modulated Oscillator");
	mNetwork.ConnectPorts( "oscillator-generator.Audio Output", "multiplier.First Audio Input" );
	mNetwork.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	mNetwork.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (mHasAudioOut)
		mNetwork.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{
	mNetwork.SetName("Modulated FileIn plus FileIn");
	mNetwork.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	mNetwork.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	mNetwork.ConnectPorts( "file-in.Output", "mixer.Input Audio_1" );
	mNetwork.ConnectPorts( "multiplier.Audio Output" , "mixer.Input Audio_0" );
	mNetwork.ConnectPorts( "mixer.Output Audio", "file-out.Input" );

	if (mHasAudioOut)
		mNetwork.ConnectPorts( "mixer.Output Audio", "audio-out.Input" );
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworkTopologies()
{
	NetworkConfigurationMethods::iterator currentConfigMethod;
	for ( currentConfigMethod=mConfigurations.begin(); 
		  currentConfigMethod != mConfigurations.end(); 
		  currentConfigMethod++ )
	{
		mNetwork.DisconnectAllPorts();
		currentConfigMethod->Configure();
		mNetwork.Start();

		for (int i=0; i<mMaxFramesToProcess; i++)
			mNetwork.DoProcessings();

		std::cout << mNetwork.GetName() << " network processed.\n";
		mNetwork.Stop();
	}
	
}

} //namespace

