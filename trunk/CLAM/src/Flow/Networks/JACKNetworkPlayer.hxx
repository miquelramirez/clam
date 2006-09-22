
#ifndef _JACK_NETWORK_PLAYER_HXX_
#define _JACK_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"

#include "Network.hxx"
#include "ExternGenerator.hxx"
#include "ExternSink.hxx"

#include <jack/jack.h>

namespace CLAM
{

class JACKNetworkPlayer : public NetworkPlayer
{
private:
	//Structures to keep information about every external input and output processing
	struct SourceJackBinding
	{
		const char* PortName() 
		{
			return jack_port_name(jackPort);
		}
		jack_port_t* jackPort;
		ExternGenerator* source;
	};

	struct SinkJackBinding
	{
		const char* PortName() 
		{
			return jack_port_name(jackPort);
		}
		jack_port_t* jackPort;
		ExternSink* sink;
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
	int mJackSampleRate, mJackBufferSize, mClamBufferSize;
	bool mAutoConnect;

	JackConnections mIncomingJackConnections;
	JackConnections mOutgoingJackConnections;

	SourceJackBindings mSourceJackBindings;
	SinkJackBindings mSinkJackBindigs;

	std::string mJackOutPortAutoConnectList, mJackInPortAutoConnectList;
	
	//JACK CODE
	jack_client_t * mJackClient;
	std::string mJackClientname;

public:
	//When created in the prototyper
	JACKNetworkPlayer(const std::string & networkFile, std::list<std::string> portlist);
	//When created in neteditor
	JACKNetworkPlayer();
	virtual ~JACKNetworkPlayer();

	void InitClient();
	
	//JACK sever (un)registration methods
	void RegisterPorts();
	void RegisterInputPorts(const Network& net);
	void RegisterOutputPorts(const Network& net);
	void UnRegisterPorts();

	//Buffer copying methods
	void CopyJackBuffersToGenerators(const jack_nframes_t nframes);
	void CopySinksToJackBuffers(const jack_nframes_t nframes);
	
	virtual void Start();
	virtual void Stop();

	virtual bool IsCallbackBased() const { return true; }

	void Do(const jack_nframes_t nframes);

	//Saves/loads the connections made to our local in/out jack ports
	void StoreConnections();
	void RestoreConnections();
	
	
	void AutoConnectPorts();
};
} //namespace CLAM

#endif
