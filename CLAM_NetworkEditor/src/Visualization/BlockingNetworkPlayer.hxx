
#ifndef _BLOCKING_NETWORK_PLAYER_HXX_
#define _BLOCKING_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"

namespace CLAM
{

class BlockingNetworkPlayer
{
private:
	CLAM::AudioManager mAudioManager;
	CLAM::Network *mNetwork;
	CLAM::Thread mThread;
	bool mStopped, mModified;
	
public:
	//Called by the prototyper
	BlockingNetworkPlayer(const std::string & networkFile)
		: mAudioManager( 44100, 512 )
		, mThread(/*realtime*/true)
		, mStopped(true)

	{
		mNetwork->AddFlowControl( new CLAM::PushFlowControl( /*frameSize*/ 512 ));
		CLAM::XmlStorage::Restore(*mNetwork,networkFile);
		mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
		mThread.SetupPriorityPolicy();
		
	}

	//Called by NetworController
	BlockingNetworkPlayer()
		: mAudioManager( 44100, 512 )
		, mThread(/*realtime*/true)
		, mStopped(true)
	{
		mNetwork=NULL;
		mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
		mThread.SetupPriorityPolicy();		
	}
	
	void Start()
	{
		if (!mStopped)
			return;
		
		mStopped=false;
		mNetwork->ReconfigureAllProcessings();		
		mNetwork->Start();
		mThread.Start();
	}
	void Do()
	{
		while (!mStopped)
		{
			mNetwork->Do();
		}
	}
	void Stop()
	{
		if (mStopped)
			return;

		mStopped=true;
		mThread.Stop();
		mNetwork->Stop();
	}
	~BlockingNetworkPlayer()
	{
		Stop();
	}

	void SetNetwork (Network& net)
	{
		mNetwork=&net;
	}
	
	Network& GetNetwork()
	{
		CLAM_ASSERT( (mNetwork!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
		return *mNetwork;
	}
	void Clear()
	{
		GetNetwork().Clear();
	}
	
	void NotifyModification()
	{
		mModified=true;	
	}

};

} //namespace CLAM

#endif
