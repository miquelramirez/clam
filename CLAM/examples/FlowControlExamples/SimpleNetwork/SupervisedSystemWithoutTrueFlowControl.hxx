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
		SupervisedSystemWithoutTrueFlowControl* mParent;
		NetworkConfigurationMethod mConfigurationMethod;
	public:
		NetworkConfigurationMethodWrapper(
			SupervisedSystemWithoutTrueFlowControl* parent,
			NetworkConfigurationMethod confMethod ) :
			mParent(parent),
			mConfigurationMethod(confMethod)
		{}
		
		void Configure()
		{ 
			(mParent->*mConfigurationMethod)();
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
	std::string mFileInName;
	std::string mFileOutName;
	int mFrameSize;
	int mMaxFramesToProcess;
	bool mHasAudioOut;

	CLAM::Network mNetwork;

	NetworkConfigurationMethods mConfigurations;

	CLAM::AudioManager mAudioManager;

	//configurations
	CLAM::OscillatorConfig mGeneratorCfg;
	CLAM::OscillatorConfig mModulatorCfg;
	CLAM::AudioFileConfig mFileInCfg;
	CLAM::AudioFileConfig mFileOutCfg;
	CLAM::AudioIOConfig mAudioOutCfg;
	CLAM::AudioMixerConfig mMixerCfg;
};

} //namespace

#endif
