#include "BlockingNetworkPlayer.hxx"

#include <iostream>
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"

namespace CLAM
{

//Called by the prototyper
BlockingNetworkPlayer::BlockingNetworkPlayer(const std::string & networkFile)
	: NetworkPlayer()
	, mAudioManager( 44100, 512 )
	, mThread(/*realtime*/true)
{

#ifdef USE_OSC
	SetNetwork( *( new OSCEnabledNetwork() ) );
#else
	SetNetwork( *( new Network() ) );
#endif

	mAudioManager.Start();
	GetNetwork().AddFlowControl( new PushFlowControl( /*frameSize*/ 512 ));
	XmlStorage::Restore( GetNetwork() ,networkFile);
	mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
	mThread.SetupPriorityPolicy();
}

//Called by NetworController
BlockingNetworkPlayer::BlockingNetworkPlayer()
	: NetworkPlayer()
	, mAudioManager( 44100, 512 )
	, mThread(/*realtime*/true)
{
	mAudioManager.Start();
	mThread.SetThreadCode( makeMemberFunctor0( *this, BlockingNetworkPlayer, Do ) );
	mThread.SetupPriorityPolicy();		
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
	GetNetwork().ReconfigureAllProcessings();		
	GetNetwork().Start();
	mThread.Start();
}

void BlockingNetworkPlayer::Stop()
{
	if ( IsStopped() )
		return;

	SetStopped(true);
	mThread.Stop();
	GetNetwork().Stop();
}

void BlockingNetworkPlayer::Do()
{
	while ( !IsStopped() )
	{
		GetNetwork().Do();
	}
}

} //namespace CLAM
