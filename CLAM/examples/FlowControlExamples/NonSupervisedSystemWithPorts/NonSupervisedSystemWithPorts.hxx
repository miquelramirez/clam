#ifndef _NonSupervisedSystem_hxx_
#define _NonSupervisedSystem_hxx_

#include "SimpleOscillator.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileIn.hxx"
#include "AudioMultiplier.hxx"
#include "AudioOut.hxx"
#include "AudioMixer.hxx"
#include "Audio.hxx"
#include "AudioManager.hxx"
#include <list>
#include <string>
#include "AutoPanner.hxx"

namespace FlowControlExample
{

// forward decl
class SystemWithPorts;

class NetworkConfiguration
{
public: 
	NetworkConfiguration( SystemWithPorts* sys, std::string name) : 
	  mSystem(sys),
	  mName(name)
	{}

	// returns true if has finished the processing (i.e. end-of-file)
	virtual bool Do() = 0;
	virtual void Connect()  =0;
	virtual void Start() {};
	virtual void Stop() {};
	const std::string& GetName() const { return mName; }
	
	void ConnectAndDo();
	virtual ~NetworkConfiguration() {};

protected:
	/// to be used in template methods definitions Do() and Connect()
	SystemWithPorts& System() { return *mSystem; }

private:
	SystemWithPorts* mSystem;
protected:
	const std::string mName;
	static bool trace;
};

//----------------------------------------------------------------------
class SystemWithPorts
{

	typedef std::list<NetworkConfiguration*> NetworkConfigurations;

public:
	virtual ~SystemWithPorts();
	SystemWithPorts( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess, 
			 bool hasAudioOut );
	
	void ProcessAllNetworkConfigurations();
	void RemoveAllNetworkConfigurations();
	void AddNetworkConfiguration( NetworkConfiguration* );
	void RegisterProcessings();

	int GetMaxFramesToProcess() const {
		return mMaxFramesToProcess;
	}
	// NetworkConfiguration classes:
	
	class OscillatorToFileOut : public NetworkConfiguration {
	public:
		OscillatorToFileOut( SystemWithPorts *parent) : 
		  NetworkConfiguration(parent, "OscillatorToFileOut") {}
		bool Do();
		void Connect();
	};
	
	class FileInFileOut : public NetworkConfiguration {
	public:
		FileInFileOut( SystemWithPorts *parent ) : 
		  NetworkConfiguration(parent, "FileInFileOut") {}
		bool Do();
		void Connect();
		void Stop();
	};
	
	class ModulatedFileIn : public NetworkConfiguration {
	public:
		ModulatedFileIn( SystemWithPorts *parent ) :  
		  NetworkConfiguration(parent, "ModulatedFileIn") {}
		bool Do();
		void Connect();
		void Stop();
	};

	class ModulatedOscillator : public NetworkConfiguration {
	public:
		ModulatedOscillator( SystemWithPorts *parent ) :  
		  NetworkConfiguration(parent, "ModulatedOscillator") {}
		bool Do();
		void Connect();
	};

	class ModulatedFileInPlusFileIn : public NetworkConfiguration {
	public:
		ModulatedFileInPlusFileIn( SystemWithPorts *parent ) : 
		  NetworkConfiguration(parent, "ModulatedFileInPlusFileIn") {}
		bool Do();
		void Connect();
		void Stop();
	};

	//outside access to Processings
	typedef std::vector<CLAM::Processing*>::iterator ProcessingIterator;
	
	ProcessingIterator FirstProcessing()
	{
		return mProcessings.begin();
	}

	ProcessingIterator LastProcessing()
	{
		return mProcessings.end();
	}


public: // just for convenience
	void StartProcessings();
	void ConfigureProcessings();
	void ConfigureData();

//methods relative to audio out
	bool AudioOutDo();
	bool AudioOutAttach(CLAM::Audio& a);

	// audio manager
	CLAM::AudioManager mAudioManager;

	// processings
	CLAM::SimpleOscillator mOscillator;
	CLAM::SimpleOscillator mModulator;
	CLAM::AudioFileIn mFileIn;
	CLAM::AudioFileOut mFileOut;
	CLAM::AudioMultiplier mMultiplier;
	CLAM::AudioOut mAudioOut;
	CLAM::AudioMixer<2> mMixer;
	CLAM::AutoPanner mControlSender;	

	// processing data
	CLAM::Audio mOscillatorData;
	CLAM::Audio mFileInData;
	CLAM::Audio mModulatorData;
	CLAM::Audio mMixerData;
	CLAM::Audio mMultiplierData;

	//other system parameters
	std::string mFileInName;
	std::string mFileOutName;
	int mFrameSize;
	int mMaxFramesToProcess;
	bool mHasAudioOut;


	//system infrastructure
	NetworkConfigurations mNetworks;
	std::vector<CLAM::Processing*> mProcessings;

};

} // namespace

#endif
