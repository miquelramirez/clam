
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
	for (int i=0; i<_system->GetMaxFramesToProcess(); i++) {
		if ( Do() ) break;
	}
	if (trace) std::cout <<"Done.\n";
	Stop();
}


SystemWithNodes::SystemWithNodes( std::string fileIn, std::string fileOut , int frameSize , int nFrames, bool hasAudioOut ) :
	_audioManager(44100, frameSize),
	_controlSender(20,44100,0,frameSize),
	_fileInName(fileIn),
	_fileOutName(fileOut), 
	_frameSize(frameSize), 
	_maxFramesToProcess(nFrames),
	_hasAudioOut(hasAudioOut),
	_oscillatorData(0),
	_fileInData(0),
	_modulatorData(0),
	_mixerData(0),
	_multiplierData(0)

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
	_processings.push_back( &_oscillator );
	_processings.push_back( &_modulator );
	_processings.push_back( &_fileIn );
	_processings.push_back( &_fileOut );
	_processings.push_back( &_multiplier );
	_processings.push_back( &_audioOut);
	_processings.push_back( &_mixer );
//	_processings.push_back( &_controlSender );
	
}

void SystemWithNodes::ConfigureProcessings()
{

	// oscillators
	CLAM::OscillatorConfig oscilCfg;
	oscilCfg.SetFrequency(440.0);
	oscilCfg.SetAmplitude(0.5);

	_oscillator.Configure(oscilCfg);

	oscilCfg.SetFrequency(220.0);
	_modulator.Configure(oscilCfg);

	// Audio File In & Out
	CLAM::AudioFileConfig fileCfg;
	fileCfg.SetFilename( _fileOutName );
	fileCfg.SetChannels(1);
	fileCfg.SetFiletype( CLAM::EAudioFileType::eWave );
	fileCfg.SetFrameSize( _frameSize );
	fileCfg.SetKeepFrameSizes(true);

	_fileOut.Configure( fileCfg );

	fileCfg.SetFilename(_fileInName);

	_fileIn.Configure (fileCfg);

	CLAM::AudioMixerConfig mixerCfg;
	mixerCfg.SetFrameSize(_frameSize);

	_mixer.Configure( mixerCfg );

	if (_hasAudioOut)
	{
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);

		_audioOut.Configure(audioCfg);
	}
}

void SystemWithNodes::StartProcessings()
{
	try{
		_oscillator.Start();
		_fileOut.Start();
		_fileIn.Start();
		_modulator.Start();
		_multiplier.Start();
		_mixer.Start();

		if (_hasAudioOut)
		{
			_audioOut.Start();
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
	System()._oscillator.GetOutPorts().Get( "Audio Output" ).Attach( *System()._oscillatorData );
	System()._fileOut.GetInPorts().Get( "Input" ).Attach( *System()._oscillatorData );
	System().ConditionalAudioOutAttach( *System()._oscillatorData );

	// configuring node with frame size
	System()._oscillatorData->Configure( System()._frameSize );
}

bool SystemWithNodes::OscillatorToFileOut::Do() 
{
	System()._oscillator.Do();
	System()._fileOut.Do();
	System().AudioOutDo();
	return false;
}

void SystemWithNodes::ModulatedFileIn::Connect()
{
	System()._fileIn.GetOutPorts().Get( "Output" ).Attach( *System()._fileInData );
	System()._modulator.GetOutPorts().Get( "Audio Output" ).Attach( *System()._modulatorData );
	System()._multiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System()._fileInData );
	System()._multiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System()._modulatorData );
	System()._multiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System()._multiplierData );
	System()._fileOut.GetInPorts().Get( "Input" ).Attach( *System()._multiplierData );
	System().ConditionalAudioOutAttach( *System()._multiplierData );

	System()._fileInData->Configure( System()._frameSize );
	System()._modulatorData->Configure( System()._frameSize );
	System()._multiplierData->Configure( System()._frameSize );
	
}
bool SystemWithNodes::ModulatedFileIn::Do()
{
	System()._fileIn.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._fileOut.Do();
	System().AudioOutDo();	

	return false;
}
void SystemWithNodes::ModulatedFileIn::Stop()
{
	System()._fileIn.Stop();
	System()._fileIn.Start();
}

void SystemWithNodes::ModulatedOscillator::Connect()
{
	System()._oscillator.GetOutPorts().Get( "Audio Output" ).Attach( *System()._oscillatorData );
	System()._modulator.GetOutPorts().Get( "Audio Output" ).Attach( *System()._modulatorData );
	System()._multiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System()._oscillatorData );
	System()._multiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System()._modulatorData );
	System()._multiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System()._multiplierData );
	System()._fileOut.GetInPorts().Get( "Input" ).Attach( *System()._multiplierData );
	System().ConditionalAudioOutAttach( *System()._multiplierData );


	System()._oscillatorData->Configure( System()._frameSize );
	System()._modulatorData->Configure( System()._frameSize );
	System()._multiplierData->Configure( System()._frameSize );

}
bool SystemWithNodes::ModulatedOscillator::Do()
{
	System()._oscillator.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._fileOut.Do();
	System().AudioOutDo();
	System().AudioOutDo();

	return false;
}

void SystemWithNodes::FileInFileOut::Connect()
{

	System()._fileIn.GetOutPorts().Get( "Output" ).Attach( *System()._fileInData );
	System()._fileOut.GetInPorts().Get( "Input" ).Attach( *System()._fileInData );
	System().ConditionalAudioOutAttach( *System()._fileInData );

	System()._fileInData->Configure( System()._frameSize );

}
bool SystemWithNodes::FileInFileOut::Do()
{
	System()._fileIn.Do();
	System()._fileOut.Do();
	System().AudioOutDo();

	return false;
}
void SystemWithNodes::FileInFileOut::Stop()
{
	System()._fileIn.Stop();
	System()._fileIn.Start();
}

void SystemWithNodes::ModulatedFileInPlusFileIn::Connect()
{
	// linking ControlSender with AudioMixer volumes.
	System()._controlSender.mLeft.AddLink(&(System()._mixer.GetInControls().Get("Input Gain_0")));
	System()._controlSender.mRight.AddLink(&(System()._mixer.GetInControls().Get("Input Gain_1")));

	// attach for each Port
	System()._fileIn.GetOutPorts().Get( "Output" ).Attach( *System()._fileInData );
	System()._modulator.GetOutPorts().Get( "Audio Output" ).Attach( *System()._modulatorData );
	System()._multiplier.GetInPorts().Get( "First Audio Input" ).Attach( *System()._fileInData );
	System()._multiplier.GetInPorts().Get( "Second Audio Input" ).Attach( *System()._modulatorData );
	System()._multiplier.GetOutPorts().Get( "Audio Output" ).Attach( *System()._multiplierData );
	System()._mixer.GetInPorts().Get("Input Audio_0").Attach( *System()._multiplierData );
	System()._mixer.GetInPorts().Get("Input Audio_1").Attach( *System()._fileInData );
	System()._mixer.GetOutPorts().Get("Output Audio").Attach( *System()._mixerData );
	System()._fileOut.GetInPorts().Get( "Input" ).Attach( *System()._mixerData );
	System().ConditionalAudioOutAttach( *System()._mixerData);

	System()._fileInData->Configure( System()._frameSize );
	System()._modulatorData->Configure( System()._frameSize );
	System()._multiplierData->Configure( System()._frameSize );
	System()._mixerData->Configure( System()._frameSize );
}
bool SystemWithNodes::ModulatedFileInPlusFileIn::Do()
{
	System()._fileIn.Do();
	System()._modulator.Do();
	System()._multiplier.Do();
	System()._controlSender.Do();
	System()._mixer.Do();
	System()._fileOut.Do();
	System().AudioOutDo();
	return false;
}
void SystemWithNodes::ModulatedFileInPlusFileIn::Stop() {
	System()._fileIn.Stop();
	System()._fileIn.Start();
}


void SystemWithNodes::AddNetworkConfiguration( NetworkConfiguration* item)
{
	_networks.push_back( item );
}
void SystemWithNodes::RemoveAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=_networks.begin(); it!=_networks.end(); it++)
		delete (*it);
}
void SystemWithNodes::ProcessAllNetworkConfigurations()
{
	NetworkConfigurations::iterator it;
	for (it=_networks.begin(); it!=_networks.end(); it++) {
		(*it)->ConnectAndDo();
	}
}


bool SystemWithNodes::AudioOutDo()
{
	if (_hasAudioOut)
	{
		_audioOut.Do();
		return true;
	}
	return false;
}

bool SystemWithNodes::ConditionalAudioOutAttach( AudioNode& a)
{
	if (_hasAudioOut)
	{
		_audioOut.GetInPorts().Get( "Input" ).Attach( a );
		return true;
	}
	return false;
}

void SystemWithNodes::DeleteAllNodes()
{
	if ( _oscillatorData ) delete _oscillatorData;
	if ( _fileInData ) delete _fileInData;
	if ( _modulatorData ) delete _modulatorData;
	if ( _mixerData ) delete _mixerData;
	if ( _multiplierData ) delete _multiplierData;
}
void SystemWithNodes::ResetAllNodes()
{
	// this is a provisional way of reset nodes! 
	// Nodes should have a Reset method
	// todo: change this method when new Node interface is ready
	DeleteAllNodes();

	_oscillatorData = new AudioNode();
    _fileInData = new AudioNode();
	_modulatorData = new AudioNode();
	_mixerData = new AudioNode();
	_multiplierData = new AudioNode();
}

void SystemWithNodes::ConfigurePorts()
{
	_oscillator.GetOutPorts().Get( "Audio Output" ).SetParams( _frameSize );
	_fileIn.GetOutPorts().Get( "Output" ).SetParams( _frameSize );
	_modulator.GetOutPorts().Get( "Audio Output" ).SetParams( _frameSize );
	_multiplier.GetInPorts().Get( "First Audio Input" ).SetParams( _frameSize );
	_multiplier.GetInPorts().Get( "Second Audio Input" ).SetParams( _frameSize );
	_multiplier.GetOutPorts().Get( "Audio Output" ).SetParams( _frameSize );
	_mixer.GetInPorts().Get("Input Audio_0").SetParams( _frameSize );
	_mixer.GetInPorts().Get("Input Audio_1").SetParams( _frameSize );
	_mixer.GetOutPorts().Get("Output Audio").SetParams( _frameSize );
	_fileOut.GetInPorts().Get( "Input" ).SetParams( _frameSize );
	if ( _hasAudioOut )
		_audioOut.GetInPorts().Get( "Input" ).SetParams( _frameSize );
}


} // namespace
