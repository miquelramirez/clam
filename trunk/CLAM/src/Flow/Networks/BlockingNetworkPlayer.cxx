#include "BlockingNetworkPlayer.hxx"

#include <iostream>
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{


//Called by NetworController
BlockingNetworkPlayer::BlockingNetworkPlayer()
	: NetworkPlayer()
	, mAudioManager( 44100, 512 )
	, mThread(/*realtime*/true)
{
	mAudioManager.Start();
	mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
}

BlockingNetworkPlayer::~BlockingNetworkPlayer()
{
	Stop();
}

void BlockingNetworkPlayer::Start()
{				
	if ( !IsStopped() )
		return;
	
	SetStopped(false);
//	GetNetwork().ReconfigureAllProcessings();//TODO try to remove this	
	mThread.Start();
}

void BlockingNetworkPlayer::Stop()
{
	if ( IsStopped() )
		return;

	SetStopped(true);
	mThread.Stop();
}

void BlockingNetworkPlayer::Do()
{
	while ( !IsStopped() )
	{
		GetNetwork().Do();
	}
}

} //namespace CLAM

