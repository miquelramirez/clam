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
	CLAM::Oscillator mOscillator;
	CLAM::Oscillator mModulator;
	CLAM::AudioFileIn mFileIn;
	CLAM::AudioFileOut mFileOut;
	CLAM::AudioAdder mAdder;
	CLAM::AudioMultiplier mMultiplier;

	CLAM::AudioOut mAudioOut;

	// processing data
	CLAM::Audio mOscillatorData;
	CLAM::Audio mFileInData;
	CLAM::Audio mModulatorData;
	CLAM::Audio mAdderData;
	CLAM::Audio mMultiplierData;

	//other system parameters
	std::string mFileInName;
	std::string mFileOutName;
	int mFrameSize;
	int mMaxFramesToProcess;
	bool mHasAudioOut;

};

} // namespace

#endif
