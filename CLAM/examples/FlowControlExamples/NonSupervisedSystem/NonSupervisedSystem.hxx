#ifndef _NonSupervisedSystem_hxx_
#define _NonSupervisedSystem_hxx_

#include "Oscillator.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioAdder.hxx"
#include "AudioMultiplier.hxx"

#include "Audio.hxx"
#include <list>
#include <string>

#include "AudioOut.hxx"

namespace FlowControlExample
{

class System
{
	// Iteration method type
	typedef bool (System::*IterationMethod)(void);

public:
	virtual ~System() {};
	System( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess, 
		bool hasAudioOut);
	
	void DoProcessings( IterationMethod );
	
	void ProcessAllIterations();

	// Iteration methods
	bool OscillatorToFileOut();
	bool ModulatedFileIn();
	bool ModulatedOscillator();
	bool ModulatedFileInPlusFileIn();
	bool FileInFileOut();
	
private:
	void StartProcessings();
	void ConfigureProcessings();
	void ConfigureData();

	// processings
	CLAM::Oscillator _oscillator;
	CLAM::Oscillator _modulator;
	CLAM::AudioFileIn _fileIn;
	CLAM::AudioFileOut _fileOut;
	CLAM::AudioAdder _adder;
	CLAM::AudioMultiplier _multiplier;

	CLAM::AudioOut _audioOut;

	// processing data
	CLAM::Audio _oscillatorData;
	CLAM::Audio _fileInData;
	CLAM::Audio _modulatorData;
	CLAM::Audio _adderData;
	CLAM::Audio _multiplierData;

	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;
	bool _hasAudioOut;

};

} // namespace

#endif
