#include "NonSupervisedSystem.hxx"

#include <iostream>

#include "AudioIO.hxx"

namespace FlowControlExample
{

System::System( std::string fileIn, std::string fileOut , int frameSize , int nFrames, bool hasAudioOut) : 
	_fileInName(fileIn),
	_fileOutName(fileOut), 
	_frameSize(frameSize), 
	_maxFramesToProcess(nFrames),
	_hasAudioOut(hasAudioOut)
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

	if (_hasAudioOut)
	{
		CLAM::AudioIOConfig audioCfg;
		audioCfg.SetFrameSize(_frameSize);

		_audioOut.Configure(audioCfg);
	}
}

void System::ConfigureData()
{
	_oscillatorData.SetSize(_frameSize);
	_fileInData.SetSize(_frameSize);
	_modulatorData.SetSize(_frameSize);
	_multiplierData.SetSize(_frameSize);
	_adderData.SetSize(_frameSize);
}

void System::StartProcessings()
{
	try{
		_oscillator.Start();
		_fileOut.Start();
		_fileIn.Start();
		_modulator.Start();
		_multiplier.Start();
		_adder.Start();
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

bool System::OscillatorToFileOut() 
{
	_oscillator.Do(_oscillatorData);
	_fileOut.Do(_oscillatorData);
	if (_hasAudioOut)
	{
		_audioOut.Do(_oscillatorData);
	}

	return false;
}

bool System::ModulatedFileIn()
{
	_fileIn.Do(_fileInData);
	_modulator.Do(_modulatorData);
	_multiplier.Do(_fileInData, _modulatorData, _multiplierData);
	_fileOut.Do(_multiplierData);
	if (_hasAudioOut)
	{
		_audioOut.Do(_multiplierData);
	}

	return false;
}

bool System::ModulatedOscillator()
{
	_oscillator.Do(_oscillatorData);
	_modulator.Do(_modulatorData);
	_multiplier.Do(_oscillatorData, _modulatorData, _multiplierData);
	_fileOut.Do(_multiplierData);
	if (_hasAudioOut)
	{
		_audioOut.Do(_multiplierData);
	}

	return false;
}

bool System::ModulatedFileInPlusFileIn()
{
	_fileIn.Do(_fileInData);
	_modulator.Do(_modulatorData);
	_multiplier.Do(_fileInData, _modulatorData, _multiplierData);
	_adder.Do(_multiplierData, _fileInData, _adderData);
	_fileOut.Do(_adderData);
	if (_hasAudioOut)
	{
		_audioOut.Do(_adderData);
	}

	return false;
}

bool System::FileInFileOut()
{
	_fileIn.Do(_fileInData);
	_fileOut.Do(_fileInData);
	if (_hasAudioOut)
	{
		_audioOut.Do(_fileInData);
	}
	return false;
}

void System::DoProcessings( IterationMethod iterationDo )
{
	for (int i=0; i<_maxFramesToProcess; i++)
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

	_fileIn.Stop();
	_fileIn.Start();
	std::cout << "modulatedfilein" << std::endl;
	DoProcessings( &System::ModulatedFileIn );

	_fileIn.Stop();
	_fileIn.Start();
	std::cout << "modulatedfileinplusfilein" << std::endl;
	DoProcessings( &System::ModulatedFileInPlusFileIn);

}

} // namespace
