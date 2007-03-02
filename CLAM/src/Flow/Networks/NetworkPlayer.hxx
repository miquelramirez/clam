#ifndef _NETWORK_PLAYER_HXX_
#define _NETWORK_PLAYER_HXX_

#include "Network.hxx"

namespace CLAM
{
	/**
	 * A NetworkPlayer is an object that controls the playback of a 
	 * Network providing a high level transport like interface.
	 * This class is an abstract class.
	 * Subclasses contextualizes the network inside a given execution
	 * context dealing with threading, callback calling and device mapping
	 * on concrete architectures such as Alsa, PortAudio, Jack, VST, Ladspa...
	 */
	class NetworkPlayer
	{
		Network *mNetwork;
		bool mStopped;
	public:
		NetworkPlayer()
		{
			SetStopped(true);
			mNetwork=NULL;
		}

		virtual ~NetworkPlayer()
		{
		}
		virtual bool IsCallbackBased() const = 0;
		/// Should return true when the backend is able to run the network
		virtual bool IsWorking() const = 0;
		/// Whenever the backend is not working, this method returns the reason
		virtual std::string NonWorkingReason() const = 0;
		
		virtual void Start()=0;
		virtual void Stop()=0;
		
		void SetNetworkBackLink( Network& net )
		{
			mNetwork=&net;
		}
	protected:
		Network& GetNetwork()
		{
			CLAM_ASSERT( (mNetwork!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
			return *mNetwork;
		}
	public:
		bool IsStopped()
		{
			return mStopped;
		}
	protected:
		void SetStopped(const bool val)
		{
			mStopped=val;
		}

	};
} //namespace CLAM

#endif // _NETWORK_PLAYER_HXX_
