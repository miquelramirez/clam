#ifndef _NonSupervisedSystemWithNodes_hxx_
#define _NonSupervisedSystemWithNodes_hxx_

#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"

#include "Oscillator.hxx"
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
	  _system(sys),
	  _name(name)
	{}

	// returns true if has finished the processing (i.e. end-of-file)
	virtual bool Do() = 0;
	virtual void Connect()  =0;
	virtual void Start() {};
	virtual void Stop() {};
	const std::string& GetName() const { return _name; }
	
	void ConnectAndDo();
	virtual ~NetworkConfiguration() {};

protected:
	/// to be used in template methods definitions Do() and Connect()
	SystemWithNodes& System() { return *_system; }

private:
	SystemWithNodes* _system;
protected:
	const std::string _name;
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
	
	int GetMaxFramesToProcess() const {
		return _maxFramesToProcess;
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
		return _processings.begin();
	}

	ProcessingIterator LastProcessing()
	{
		return _processings.end();
	}


private:

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
	CLAM::AudioManager _audioManager;

	// processings
	CLAM::Oscillator _oscillator;
	CLAM::Oscillator _modulator;
	CLAM::AudioFileIn _fileIn;
	CLAM::AudioFileOut _fileOut;
	CLAM::AudioMultiplier _multiplier;
	CLAM::AudioOut _audioOut;
	CLAM::AudioMixer<2> _mixer;
	CLAM::AutoPanner _controlSender;	


	AudioNode* _oscillatorData;
	AudioNode* _fileInData;
	AudioNode* _modulatorData;
	AudioNode* _mixerData;
	AudioNode* _multiplierData;
	
	//other system parameters
	std::string _fileInName;
	std::string _fileOutName;
	int _frameSize;
	int _maxFramesToProcess;
	bool _hasAudioOut;


	//system infrastructure
	NetworkConfigurations _networks;
	std::vector<CLAM::Processing*> _processings;

};

} // namespace

#endif
