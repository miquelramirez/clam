#ifndef JACKNetworkPlayer_hxx
#define JACKNetworkPlayer_hxx

#include <iostream>
#include <string>
#include <vector>
#include "NetworkPlayer.hxx"
#include "Network.hxx"
#include <jack/jack.h>

namespace CLAM
{

class JACKNetworkPlayer : public NetworkPlayer
{
private:
	struct JackPort 
	{
		const char* PortName() { return jack_port_name(jackPort); }
		jack_port_t* jackPort;
	};
	
	typedef std::vector<JackPort> JackPorts;

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

	JackPorts _sourceJackPorts;
	JackPorts _sinkJackPorts;

	JackConnections _incomingJackConnections;
	JackConnections _outgoingJackConnections;

	std::string _jackOutPortAutoConnectList;
	std::string _jackInPortAutoConnectList;
	
	//JACK CODE
	jack_client_t * _jackClient;
	std::string _jackClientName;

private:
	void AddSourceJackBinding(Network::AudioSources::const_iterator& it, 
							  std::string const& name, unsigned index);

public:
	JACKNetworkPlayer(const std::string & name="CLAM network player");
	virtual ~JACKNetworkPlayer();

	void InitClient();
	bool IsWorking();

	virtual bool IsRealTime() const { return true; }

	std::string NonWorkingReason();
	
	//JACK sever (un)registration methods
	void RegisterPorts();
	void RegisterInputPorts(const Network& net);
	void RegisterOutputPorts(const Network& net);
	void UnRegisterPorts();
	void OnShutdown();

	//Buffer copying methods
	void CopyJackBuffersToGenerators(const jack_nframes_t nframes);
	void CopySinksToJackBuffers(const jack_nframes_t nframes);
	void BlankJackBuffers(const jack_nframes_t nframes);
	
	virtual void Start();
	virtual void Stop();
	virtual void Init();
	
	virtual unsigned BackendBufferSize() 
	{
		return _jackBufferSize; 
	}

	virtual unsigned BackendSampleRate() { return _jackSampleRate; }
	
	void Do(const jack_nframes_t nframes);

	//Saves/loads the connections made to our local in/out jack ports
	void StoreConnections();
	void RestoreConnections();
	
	
	void AutoConnectPorts();
};
} //namespace CLAM

#endif

