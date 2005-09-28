
#ifndef _BLOCKING_NETWORK_PLAYER_HXX_
#define _BLOCKING_NETWORK_PLAYER_HXX_

#include <iostream>
#include <string>
#include "NetworkPlayer.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"

namespace CLAM
{

class BlockingNetworkPlayer : public NetworkPlayer
{
private:
	CLAM::AudioManager mAudioManager;
	CLAM::Thread mThread;

	
public:
	//Called by the prototyper
	BlockingNetworkPlayer(const std::string & networkFile)
		: NetworkPlayer()
		, mAudioManager( 44100, 512 )
		, mThread(/*realtime*/true)
	{
		mAudioManager.Start();
		GetNetwork().AddFlowControl( new CLAM::PushFlowControl( /*frameSize*/ 512 ));
		CLAM::XmlStorage::Restore( GetNetwork() ,networkFile);
		mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
		mThread.SetupPriorityPolicy();
	}

	//Called by NetworController
	BlockingNetworkPlayer()
		: NetworkPlayer()
		, mAudioManager( 44100, 512 )
		, mThread(/*realtime*/true)
	{
		mAudioManager.Start();
		mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
		mThread.SetupPriorityPolicy();		
	}
	
	virtual void Start()
	{				
		if ( !IsStopped() )
			return;
		
		SetStopped(false);
		GetNetwork().ReconfigureAllProcessings();		
		GetNetwork().Start();
		mThread.Start();
	}
	
	void Do()
	{
		while ( !IsStopped() )
		{
			GetNetwork().Do();
		}
	}

	virtual ~BlockingNetworkPlayer()
	{
		Stop();
	}
	
	virtual void Stop()
	{
		if ( IsStopped() )
			return;

		SetStopped(true);
		mThread.Stop();
		GetNetwork().Stop();
	}
};

} //namespace CLAM

#endif
