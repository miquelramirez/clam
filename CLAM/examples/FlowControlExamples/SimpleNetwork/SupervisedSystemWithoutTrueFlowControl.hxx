#ifndef _SupervisedSystemWithoutTrueFlowControl_hxx_
#define _SupervisedSystemWithoutTrueFlowControl_hxx_

#include "AudioManager.hxx"
#include "Network.hxx"
#include "Oscillator.hxx"
#include "AudioFile.hxx"
#include "AudioOut.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioMixer.hxx"
#include <list>

namespace FlowControlExample
{

class SupervisedSystemWithoutTrueFlowControl
{
	typedef void (SupervisedSystemWithoutTrueFlowControl::*NetworkConfigurationMethod) ();

	class NetworkConfigurationMethodWrapper
	{
		SupervisedSystemWithoutTrueFlowControl* _parent;
		NetworkConfigurationMethod _configurationMethod;
	public:
		NetworkConfigurationMethodWrapper(
			SupervisedSystemWithoutTrueFlowControl* parent,
			NetworkConfigurationMethod confMethod ) :
			_parent(parent),
			_configurationMethod(confMethod)
		{}
		
		void Configure()
		{ 
			(_parent->*_configurationMethod)();
		}
	};

	typedef std::list<NetworkConfigurationMethodWrapper> NetworkConfigurationMethods;

public:	
	SupervisedSystemWithoutTrueFlowControl( 
		std::string fileIn, 
		std::string fileOut, 
		int frameSize, 
		int maxFramesToProcess, 
		bool hasAudioOut );

	virtual ~SupervisedSystemWithoutTrueFlowControl(){}

	//! Deprecated method
	void ConfigureNetworks();
	//! Deprecated method
	void ProcessAllNetworks();

	void ProcessAllNetworkTopologies();

private:

	void ConfigureAndAddProcessingsToNetwork();
	void RegisterAllNetworkConfigurationMethods();
	
	// ConfigureAndAddProcessingsToNetwork helper methods
	CLAM::AudioFileConfig CreateFileInCfg();
	CLAM::OscillatorConfig CreateGeneratorCfg();
	CLAM::OscillatorConfig CreateModulatorCfg();
	CLAM::AudioMixerConfig CreateMixerCfg();
	CLAM::AudioFileConfig CreateFileOutCfg();
	CLAM::AudioIOConfig CreateAudioOutCfg();

	void ConfigureOscillatorToFileOut();
	void ConfigureFileInFileOut();
	void ConfigureModulatedFileIn();
	void ConfigureModulatedOscillator();
	void ConfigureModulatedFileInPlusFileIn();

	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;
	bool _hasAudioOut;

	CLAM::Network _network;

	NetworkConfigurationMethods _configurations;

	CLAM::AudioManager _audioManager;

	//configurations
	CLAM::OscillatorConfig _generatorCfg;
	CLAM::OscillatorConfig _modulatorCfg;
	CLAM::AudioFileConfig _fileInCfg;
	CLAM::AudioFileConfig _fileOutCfg;
	CLAM::AudioIOConfig _audioOutCfg;
	CLAM::AudioMixerConfig _mixerCfg;
};

} //namespace

#endif
