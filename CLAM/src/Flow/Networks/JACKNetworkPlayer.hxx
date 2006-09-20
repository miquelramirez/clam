
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
	struct JACKOutPortCouple
	{
		std::string portName;
		jack_port_t* jackOutPort;
		ExternGenerator* clamReceiver;
		const char** connectedTo;
	};

	struct JACKInPortCouple
	{
		std::string portName;
		jack_port_t* jackInPort;
		ExternSink* clamSender;
		const char** connectedTo;
	};
	typedef std::vector<JACKOutPortCouple> JACKOutPortList;
	typedef std::vector<JACKInPortCouple> JACKInPortList;
private:
	int mJackSampleRate, mJackBufferSize, mClamBufferSize;
	bool mAutoConnect;

	JACKOutPortList mReceiverList;
	JACKInPortList mSenderList;
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

	void Do(const jack_nframes_t nframes);

	//Saves/loads the connections made to our local in/out jack ports
	void StoreConnections();
	void RestoreConnections();
	
	
	void AutoConnectPorts();
};
} //namespace CLAM

#endif
