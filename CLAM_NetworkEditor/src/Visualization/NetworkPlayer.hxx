#ifndef _NETWORK_PLAYER_HXX_
#define _NETWORK_PLAYER_HXX_

#include "Network.hxx"

namespace CLAM
{
	class NetworkPlayer
	{
		Network *mNetwork;
		bool mModified, mStopped;	
	public:
		NetworkPlayer()
		{
			SetStopped(true);
			SetModified(true);
			mNetwork=NULL;
		}

		virtual ~NetworkPlayer()
		{
			std::cerr << " *\t~NETWORKPLAYER"<<std::endl;
			if (mNetwork)
				delete mNetwork;
		}
		
		virtual void Start()=0;
		virtual void Stop()=0;
		virtual void Clear()
		{
			std::cerr << " *\t\tBLOCKINGNETWORKPLAYER::CLEAR"<<std::endl;
			Stop();
			GetNetwork().Clear();
			NotifyModification();
		}
	
		void SetNetwork( Network& net )
		{
			mNetwork=&net;
			NotifyModification();
		}
		Network& GetNetwork()
		{
			CLAM_ASSERT( (mNetwork!=NULL), "NetworkPlayer::GetNetwork() : NetworkPlayer does not have any Network");
			return *mNetwork;
		}

		void NotifyModification()
		{
			SetModified(true);
		}
		bool IsModified()
		{
			return mModified;
		}

		bool IsStopped()
		{
			return mStopped;
		}
	protected:
		// SetStopped -> Stop
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
