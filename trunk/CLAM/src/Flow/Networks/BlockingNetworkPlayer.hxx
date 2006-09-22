
#ifndef _BLOCKING_NETWORK_PLAYER_HXX_
#define _BLOCKING_NETWORK_PLAYER_HXX_

#include <string>
#include "NetworkPlayer.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"

namespace CLAM
{

class BlockingNetworkPlayer : public NetworkPlayer
{
private:
	AudioManager mAudioManager;
	Thread mThread;
	
public:
	//When created in the prototyper
	BlockingNetworkPlayer(const std::string & networkFile);
	//When created in neteditor
	BlockingNetworkPlayer();
	
	virtual ~BlockingNetworkPlayer();
	virtual bool IsCallbackBased() const { return false; }
	
	virtual void Start();
	virtual void Stop();

	void Do();


};

} //namespace CLAM

#endif
