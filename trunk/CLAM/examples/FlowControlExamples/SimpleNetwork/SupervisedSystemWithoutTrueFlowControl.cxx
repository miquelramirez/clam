
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

	_audioManager(44100, frameSize),
	_frameSize(frameSize), 
	_maxFramesToProcess(maxFramesToProcess),
	_hasAudioOut(hasAudioOut),
	_fileInName(fileIn),
	_fileOutName(fileOut)
{
	_network.AddFlowControl( new CLAM::PushFlowControl( _frameSize ));
	ConfigureAndAddProcessingsToNetwork();
	RegisterAllNetworkConfigurationMethods();
}


void SupervisedSystemWithoutTrueFlowControl::ConfigureAndAddProcessingsToNetwork()
{
	_network.AddProcessing("file-in", new CLAM::AudioFileIn( CreateFileInCfg() ));
	_network.AddProcessing("oscillator-generator", new CLAM::Oscillator( CreateGeneratorCfg() ) );
	_network.AddProcessing("oscillator-modulator", new CLAM::Oscillator( CreateModulatorCfg() ) );
	_network.AddProcessing("multiplier", new CLAM::AudioMultiplier );
	_network.AddProcessing( "mixer", new CLAM::AudioMixer<2>( CreateMixerCfg() ) );
	_network.AddProcessing("file-out", new CLAM::AudioFileOut( CreateFileOutCfg() ) );
	//_network.AddProcessing("audio-out", new CLAM::AudioOut( CreateAudioOutCfg() ) );
}

CLAM::AudioFileConfig SupervisedSystemWithoutTrueFlowControl::CreateFileInCfg()
{
	CLAM::AudioFileConfig cfg;
	cfg.SetChannels(1);
	cfg.SetFiletype( CLAM::EAudioFileType::eWave );
	cfg.SetFrameSize( _frameSize );
	cfg.SetFilename( _fileInName );
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
	cfg.SetFrameSize(_frameSize);
	return cfg;
}
CLAM::AudioFileConfig SupervisedSystemWithoutTrueFlowControl::CreateFileOutCfg()
{
	CLAM::AudioFileConfig cfg;
	cfg.SetChannels(1);
	cfg.SetFiletype( CLAM::EAudioFileType::eWave );
	cfg.SetFrameSize( _frameSize );
	cfg.SetFilename( _fileOutName );
	cfg.SetKeepFrameSizes(true);
	return cfg;
}
CLAM::AudioIOConfig SupervisedSystemWithoutTrueFlowControl::CreateAudioOutCfg()
{
	CLAM::AudioIOConfig cfg;
	cfg.SetFrameSize(_frameSize);
	return cfg;
}
//---------------------------------------------------------------------------------


void SupervisedSystemWithoutTrueFlowControl::RegisterAllNetworkConfigurationMethods()
{

	_configurations.push_back( NetworkConfigurationMethodWrapper(
		this, &SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut ) );
	_configurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator ) );
	_configurations.push_back( 	NetworkConfigurationMethodWrapper(
		this, &SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut ) );
	_configurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn ) );
	_configurations.push_back( NetworkConfigurationMethodWrapper(
		this,&SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn ) );

}

void SupervisedSystemWithoutTrueFlowControl::ConfigureOscillatorToFileOut()
{
	_network.SetName("Oscillator to FileOut");
	_network.ConnectPorts( "oscillator-generator.Audio Output", "file-out.Input" );

	if (_hasAudioOut)
		_network.ConnectPorts( "oscillator-generator.Audio Output", "audio-out.Input" );
	
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureFileInFileOut()
{
	_network.SetName("FileIn to FileOut");
	_network.ConnectPorts( "file-in.Output", "file-out.Input" );
	
	if (_hasAudioOut)
		_network.ConnectPorts( "file-in.Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileIn()
{
	_network.SetName("Modulated FileIn");
	_network.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_network.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_network.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
		_network.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedOscillator()
{
	_network.SetName("Modulated Oscillator");
	_network.ConnectPorts( "oscillator-generator.Audio Output", "multiplier.First Audio Input" );
	_network.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_network.ConnectPorts( "multiplier.Audio Output", "file-out.Input" );	

	if (_hasAudioOut)
		_network.ConnectPorts( "multiplier.Audio Output", "audio-out.Input" );
}
void SupervisedSystemWithoutTrueFlowControl::ConfigureModulatedFileInPlusFileIn()
{
	_network.SetName("Modulated FileIn plus FileIn");
	_network.ConnectPorts( "file-in.Output", "multiplier.First Audio Input" );
	_network.ConnectPorts( "oscillator-modulator.Audio Output", "multiplier.Second Audio Input" );
	_network.ConnectPorts( "file-in.Output", "mixer.Input Audio_1" );
	_network.ConnectPorts( "multiplier.Audio Output" , "mixer.Input Audio_0" );
	_network.ConnectPorts( "mixer.Output Audio", "file-out.Input" );

	if (_hasAudioOut)
		_network.ConnectPorts( "mixer.Output Audio", "audio-out.Input" );
}

void SupervisedSystemWithoutTrueFlowControl::ProcessAllNetworkTopologies()
{
	NetworkConfigurationMethods::iterator currentConfigMethod;
	for ( currentConfigMethod=_configurations.begin(); 
		  currentConfigMethod != _configurations.end(); 
		  currentConfigMethod++ )
	{
		_network.DisconnectAllPorts();
		currentConfigMethod->Configure();
		_network.Start();

		for (int i=0; i<_maxFramesToProcess; i++)
			_network.DoProcessings();

		std::cout << _network.GetName() << " network processed.\n";
		_network.Stop();
	}
	
}

} //namespace

