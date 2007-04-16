
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
	//When created in neteditor
	BlockingNetworkPlayer();
	
	virtual ~BlockingNetworkPlayer();
	virtual bool IsCallbackBased() const { return false; }
	virtual bool IsWorking() const { return true; }
	virtual std::string NonWorkingReason() const { return ""; }
	
	virtual void Start();
	virtual void Stop();

	void Do();


};

} //namespace CLAM

#endif

