
#ifndef _BLOCKING_NETWORK_PLAYER_HXX_
#define _BLOCKING_NETWORK_PLAYER_HXX_

#include <iostream>
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "Thread.hxx"
#include "AudioManager.hxx"
#include "QtSlot2Control.hxx"


namespace CLAM
{

class NetworkPlayer
{
	CLAM::AudioManager _audioManager;
	CLAM::Network _network;
	CLAM::Thread _thread;
	bool _stopped;
public:
	NetworkPlayer(const std::string & networkFile)
		: _audioManager( 44100, 512 )
		, _thread(/*realtime*/true)
		, _stopped(true)

	{
		_network.AddFlowControl( new CLAM::PushFlowControl( /*frameSize*/ 512 ));
		CLAM::XmlStorage::Restore(_network,networkFile);
		_thread.SetThreadCode( makeMemberFunctor0( *this, NetworkPlayer, Do ) );
	}
	void Start()
	{
		_stopped=false;
		_network.Start();
		_thread.Start();
	}
	void Do()
	{
		while (!_stopped)
		{
			_network.Do();
		}
	}
	void Stop()
	{
		_stopped=true;
		_thread.Stop();
		_network.Stop();
	}
	~NetworkPlayer()
	{
		Stop();
	}
	CLAM::Network & Network()
	{
		return _network;
	}
};

} //namespace CLAM

#endif
