#include "APlayer.hxx"
#include "QtAudioPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAudioPlayer::QtAudioPlayer(QWidget* parent) : QtPlayer(parent)
		{
			SetPlayer(new APlayer());
		}
		
		QtAudioPlayer::~QtAudioPlayer()
		{
		}

		void QtAudioPlayer::SetData(std::vector<Audio> data)
		{
			((APlayer*)_player)->SetData(data);
		}

		void QtAudioPlayer::SetLeftChannelMuted(bool b)
		{
			((APlayer*)_player)->SetLeftChannelMuted(b);
		}

		void QtAudioPlayer::SetRightChannelMuted(bool b)
		{
			((APlayer*)_player)->SetRightChannelMuted(b);
		}
	}
}

// END

