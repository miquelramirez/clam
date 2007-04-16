#include "Player.hxx"
#include "PlayableList.hxx"
#include "QtSinglePlayer.hxx"

namespace CLAM
{
    namespace VM
    {
		QtSinglePlayer::QtSinglePlayer(QWidget* parent) 
			: QtPlayer(parent)
			, mPlayer(0)
		{
		}
		
		QtSinglePlayer::~QtSinglePlayer()
		{
			if(mPlayer) delete mPlayer;
		}
		
		void QtSinglePlayer::SetPlayer(Player* player)
		{
			mPlayer = player;
		}

		void QtSinglePlayer::play()
		{
			if(mPlayer)
			{
				PlayableList::Stop();
				mPlayer->Play();
			}
		}
		
		void QtSinglePlayer::pause()
		{
			if(mPlayer) mPlayer->Pause();
		}
		
		void QtSinglePlayer::stop()
		{
			if(mPlayer) mPlayer->Stop();
		}
		
   
		void QtSinglePlayer::SetPlaySegment(const MediaTime& time)
		{
			if(mPlayer) mPlayer->SetBounds(time);
		}

		bool QtSinglePlayer::IsPlaying()
		{
			if(!mPlayer) return false;
			return mPlayer->IsPlaying();
		}

		bool QtSinglePlayer::IsRunning()
		{
			if(!mPlayer) return false;
			return mPlayer->IsRunning();
		}

		void QtSinglePlayer::SetSlotPlayingTime(Slotv1<TData>& slot)
		{
			if(mPlayer) mPlayer->SetSlotPlayingTime(slot);
		}

		void QtSinglePlayer::SetSlotStopPlaying(Slotv1<TData>& slot)
		{
			if(mPlayer) mPlayer->SetSlotStopPlaying(slot);
		}
		
    }
}

// END

