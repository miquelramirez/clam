#include "QtPlayer.hxx"
#include "PlayableList.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayablePlot::PlayablePlot() 
		{
			_player = NULL;
			PlayableList::Add(this);
		}

		PlayablePlot::~PlayablePlot()
		{
			if(_player) delete _player;
		}
				
		void PlayablePlot::Play()
		{
			if(_player) _player->play();
		}

		void PlayablePlot::Pause()
		{
			if(_player) _player->pause();
		}

		void PlayablePlot::Stop()
		{
			if(_player) _player->stop();
		}
				
		bool PlayablePlot::IsPlaying()
		{
			if(_player) return _player->IsPlaying();
			return false;
		}

		void PlayablePlot::RemoveFromPlayList()
		{
			PlayableList::Remove(this);
		}
	}
}

// END

