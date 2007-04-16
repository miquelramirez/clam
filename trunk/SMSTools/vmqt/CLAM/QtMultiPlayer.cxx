#include "Player.hxx"
#include "PlayableList.hxx"
#include "QtMultiPlayer.hxx"

namespace CLAM
{
    namespace VM
    {
		QtMultiPlayer::QtMultiPlayer(QWidget* parent) 
			: QtPlayer(parent)
			, mCurrentPlayer(0)
			, mAllPlayers(false)
		{
		}
		
		QtMultiPlayer::~QtMultiPlayer()
		{
		}
		
		void QtMultiPlayer::AddPlayer(Player* player)
		{
			mPlayers.push_back(player);
		}

		void QtMultiPlayer::play()
		{
			if(!mPlayers.size()) return;
			PlayableList::Stop();
			if(!mAllPlayers)
			{
				mPlayers[mCurrentPlayer]->Play();
			}
			else
			{
				for(unsigned i=0; i < mPlayers.size(); i++)
				{
					mPlayers[i]->Play();
				}
			}
		}
		
		void QtMultiPlayer::pause()
		{
			if(!mPlayers.size()) return; 
			if(!mAllPlayers)
			{
				mPlayers[mCurrentPlayer]->Pause();
			}
			else
			{
				for(unsigned i=0; i < mPlayers.size(); i++)
				{
					mPlayers[i]->Pause();
				}
			}
		}
		
		void QtMultiPlayer::stop()
		{
			if(!mPlayers.size()) return; 
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				if(mPlayers[i]->IsPlaying()) mPlayers[i]->Stop();
			}
		}
		
   
		void QtMultiPlayer::SetPlaySegment(const MediaTime& time)
		{
			if(!mPlayers.size()) return;
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				mPlayers[i]->SetBounds(time);
			}
		}

		bool QtMultiPlayer::IsPlaying()
		{
			if(!mPlayers.size()) return false;
			bool playing = false;
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				if(mPlayers[i]->IsPlaying()) 
				{
					playing=true;
					break;
				}
			}
			return playing;
		}

		bool QtMultiPlayer::IsRunning()
		{
			if(!mPlayers.size()) return false;
			bool running = false;
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				if(mPlayers[i]->IsRunning()) 
				{
					running=true;
					break;
				}
			}
			return running;
		}

		void QtMultiPlayer::AllPlayers(bool all)
		{
			// be careful
			mAllPlayers = all;
		}

		void QtMultiPlayer::SetSlotPlayingTime(Slotv1<TData>& slot)
		{
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				mPlayers[i]->SetSlotPlayingTime(slot);
			}
		}

		void QtMultiPlayer::SetSlotStopPlaying(Slotv1<TData>& slot)
		{
			for(unsigned i=0; i < mPlayers.size(); i++)
			{
				mPlayers[i]->SetSlotStopPlaying(slot);
			}  
		}
		
    }
}

// END

