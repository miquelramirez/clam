#ifndef JACKNetworkPlayer_hxx
#define JACKNetworkPlayer_hxx

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"
#include "Network.hxx"
#include <jack/jack.h>

namespace CLAM
{
class JACKNetworkPlayer : public NetworkPlayer
{
private:
	//Structures to keep information about every external input and output processing
	struct SourceJackBinding //TODO use mAudioSources/Sinks in the parent class instead.
	{
		const char* PortName() 
		{
			return jack_port_name(jackPort);
		}
		jack_port_t* jackPort;
		AudioSource* source;
	};

	struct SinkJackBinding
	{
		const char* PortName() 
		{
			return jack_port_name(jackPort);
		}
		jack_port_t* jackPort;
		AudioSink* sink;
	};
	typedef std::vector<SourceJackBinding> SourceJackBindings;
	typedef std::vector<SinkJackBinding> SinkJackBindings;
	struct JackConnection
	{
		std::string processingName;
		const char ** outsideConnections;
	};
	typedef std::list<JackConnection> JackConnections;
private:
	int _jackSampleRate;
	int _jackBufferSize;
	bool _autoConnect;

	JackConnections _incomingJackConnections;
	JackConnections _outgoingJackConnections;

	SourceJackBindings _sourceJackBindings;
	SinkJackBindings _sinkJackBindings;

	std::string _jackOutPortAutoConnectList;
	std::string _jackInPortAutoConnectList;
	
	//JACK CODE
	jack_client_t * _jackClient;
	std::string _jackClientName;

public:
	JACKNetworkPlayer(const std::string & name="CLAM network player");
	virtual ~JACKNetworkPlayer();

	void InitClient();
	bool IsWorking() const;
	std::string NonWorkingReason() const;
	
	//JACK sever (un)registration methods
	void RegisterPorts();
	void RegisterInputPorts(const Network& net);
	void RegisterOutputPorts(const Network& net);
	void UnRegisterPorts();
	void OnShutdown();

	//Buffer copying methods
	void CopyJackBuffersToGenerators(const jack_nframes_t nframes);
	void CopySinksToJackBuffers(const jack_nframes_t nframes);
	
	virtual void Start();
	virtual void Stop();
	virtual void Init();
	virtual unsigned BackendBufferSize() 
	{
		std::cout << "JACKNetworkPlayer::BackendBufferSize" << std::endl;
		return _jackBufferSize; 
	}
	virtual unsigned BackendSampleRate() { return _jackSampleRate; }
	
	virtual bool IsCallbackBased() const { return true; }

	void Do(const jack_nframes_t nframes);

	//Saves/loads the connections made to our local in/out jack ports
	void StoreConnections();
	void RestoreConnections();
	
	
	void AutoConnectPorts();
};
} //namespace CLAM

#endif

