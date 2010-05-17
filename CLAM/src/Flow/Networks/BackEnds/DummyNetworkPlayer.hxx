#ifndef DummyNetworkPlayer_hxx
#define DummyNetworkPlayer_hxx

#include "NetworkPlayer.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"

namespace CLAM
{

	class DummyNetworkPlayer : public NetworkPlayer
	{

	public:
		DummyNetworkPlayer()	{}
		~DummyNetworkPlayer()	{}

		virtual bool IsRealTime() const 	{ return false; }
		virtual void Start();
		virtual void Stop();


		virtual bool IsWorking() 				{return true;}
		virtual std::string NonWorkingReason()	{return "Non available.";}

	};

}
#endif

