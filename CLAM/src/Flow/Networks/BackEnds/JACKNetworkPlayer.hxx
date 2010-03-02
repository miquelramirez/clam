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
	typedef std::vector<jack_port_t*> JackPorts;
	const char * PortName(jack_port_t * port)
	{
		return jack_port_name(port);
	}
	/** Memento to restore jack connection status */
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
	void CopyJackBuffersToSources(const jack_nframes_t nframes);
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

