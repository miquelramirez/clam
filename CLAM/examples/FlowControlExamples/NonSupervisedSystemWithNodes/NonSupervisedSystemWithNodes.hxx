#ifndef _NonSupervisedSystemWithNodes_hxx_
#define _NonSupervisedSystemWithNodes_hxx_

#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"

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
class SystemWithNodes;

class NetworkConfiguration
{
public: 
	NetworkConfiguration( SystemWithNodes* sys, std::string name ) : 
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
	SystemWithNodes& System() { return *mSystem; }

private:
	SystemWithNodes* mSystem;
protected:
	const std::string mName;
	static bool trace;
};

//----------------------------------------------------------------------
class SystemWithNodes
{

	typedef std::list<NetworkConfiguration*> NetworkConfigurations;

public:
	virtual ~SystemWithNodes();
	SystemWithNodes( std::string fileIn, std::string fileOut , int frameSize, int maxFramesToProcess, 
			 bool hasAudioOut );
	
	void ProcessAllNetworkConfigurations();
	void RemoveAllNetworkConfigurations();
	void AddNetworkConfiguration( NetworkConfiguration* );
	void RegisterProcessings();
	void ResetAllNodes();
	void UnattachAllPorts();

	int GetMaxFramesToProcess() const {
		return mMaxFramesToProcess;
	}
	// NetworkConfiguration classes:
	
	class OscillatorToFileOut : public NetworkConfiguration {
	public:
		OscillatorToFileOut( SystemWithNodes *parent) : 
		  NetworkConfiguration(parent, "OscillatorToFileOut") {}
		bool Do();
		void Connect();
	};
	
	class FileInFileOut : public NetworkConfiguration {
	public:
		FileInFileOut( SystemWithNodes *parent ) : 
		  NetworkConfiguration(parent, "FileInFileOut") {}
		bool Do();
		void Connect();
		void Stop();
	};
	
	class ModulatedFileIn : public NetworkConfiguration {
	public:
		ModulatedFileIn( SystemWithNodes *parent ) :  
		  NetworkConfiguration(parent, "ModulatedFileIn") {}
		bool Do();
		void Connect();
		void Stop();
	};

	class ModulatedOscillator : public NetworkConfiguration {
	public:
		ModulatedOscillator( SystemWithNodes *parent ) :  
		  NetworkConfiguration(parent, "ModulatedOscillator") {}
		bool Do();
		void Connect();
	};

	class ModulatedFileInPlusFileIn : public NetworkConfiguration {
	public:
		ModulatedFileInPlusFileIn( SystemWithNodes *parent ) : 
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

	// processing data
	typedef CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  AudioNode;

	void StartProcessings();
	void ConfigureProcessings();
	void ConfigurePorts();
	void DeleteAllNodes();

	//methods relative to audio out
	bool AudioOutDo();
	bool ConditionalAudioOutAttach( AudioNode& a);


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


	AudioNode* mOscillatorData;
	AudioNode* mFileInData;
	AudioNode* mModulatorData;
	AudioNode* mMixerData;
	AudioNode* mMultiplierData;
	
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
