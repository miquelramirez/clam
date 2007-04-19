#include <CLAM/Message.hxx>
#include <CLAM/APlayer.hxx>
#include <CLAM/FundPlayer.hxx>
#include <CLAM/QtSMSPlayer.hxx>

namespace CLAM
{
	namespace VM
	{
		QtSMSPlayer::QtSMSPlayer(QWidget* parent)
			: QtMultiPlayer(parent)
			, mHasAudio(false)
			, mHasFundamental(false)
		{
			AddPlayer(new APlayer());
			AddPlayer(new FundPlayer());
			AllPlayers(false);
		}

		QtSMSPlayer::~QtSMSPlayer()
		{
		}

		void QtSMSPlayer::SetData(std::vector<const Audio*> data, bool setTime)
		{
			((APlayer*)mPlayers[AUDIO])->SetData(data,setTime);
			mHasAudio = true;
		}

		void QtSMSPlayer::SetData(const Segment& segment)
		{
			((FundPlayer*)mPlayers[FUNDAMENTAL])->SetData(segment);
			mHasFundamental = true;
		}
			
		void QtSMSPlayer::SetLeftChannelMuted(bool b)
		{
			((APlayer*)mPlayers[AUDIO])->SetLeftChannelMuted(b);
		}

		void QtSMSPlayer::SetRightChannelMuted(bool b)
		{
			((APlayer*)mPlayers[AUDIO])->SetRightChannelMuted(b);
		}

		void QtSMSPlayer::Flush()
		{
			stop();
			mHasAudio = false;
			mHasFundamental = false;
		}

		void QtSMSPlayer::play()
		{
			if(mCurrentPlayer < AUDIO || mCurrentPlayer > FUNDAMENTAL) return;
			int sentinel = LookIfAvailable();
			switch(sentinel)
			{
				case AUDIO:
					CLAM::VM::Message(QMessageBox::Critical,"QtSMSPlayer","The player has not audio data to play.");
					break;
				case FUNDAMENTAL:
					CLAM::VM::Message(QMessageBox::Critical,"QtSMSPlayer","The player has not fundamental data to play.");
					break;
				default:
					QtMultiPlayer::play();
					break;
			}
			
		}

		void QtSMSPlayer::SetCurrentPlayer(int playerID)
		{
			if(playerID < AUDIO || playerID > FUNDAMENTAL)
			{
				CLAM::VM::Message(QMessageBox::Critical,"QtSMSPlayer","Player id out of range.");
			}
			else
			{
				mCurrentPlayer = playerID;
			}
		}

		int QtSMSPlayer::LookIfAvailable()
		{
			if(mCurrentPlayer == AUDIO)
			{
				if(!mHasAudio) return AUDIO;
			}
			if(mCurrentPlayer == FUNDAMENTAL)
			{
				if(!mHasFundamental) return FUNDAMENTAL;
			}
			return PLAY;
		}
	}
}

// END

