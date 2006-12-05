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
		bool mModified;
		bool mStopped;
	public:
		NetworkPlayer()
		{
			SetStopped(true);
			SetModified(true);
			mNetwork=NULL;
		}

		virtual ~NetworkPlayer()
		{
		}
		virtual bool IsCallbackBased() const = 0;
		
		virtual void Start()=0;
		virtual void Stop()=0;
		
		void SetNetwork( Network& net )
		{
			mNetwork=&net;
			SetModified(true);
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
		bool IsModified()
		{
			return mModified;
		}
		void SetStopped(const bool val)
		{
			mStopped=val;
		}
		
		void SetModified(const bool mod)
		{
			mModified=mod;
		}

	};
} //namespace CLAM

#endif // _NETWORK_PLAYER_HXX_
