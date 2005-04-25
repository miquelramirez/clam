#include "Player.hxx"
#include "PlayableList.hxx"
#include "QtSinglePlayer.hxx"

namespace CLAM
{
    namespace VM
    {
	QtSinglePlayer::QtSinglePlayer(QWidget* parent) 
	    : QtPlayer(parent),
	      _player(0)
	{
	}
		
	QtSinglePlayer::~QtSinglePlayer()
	{
	    if(_player) delete _player;
	}
		
	void QtSinglePlayer::SetPlayer(Player* player)
	{
	    _player = player;
	}

	void QtSinglePlayer::play()
	{
	    if(_player)
	    {
		PlayableList::Stop();
		_player->Play();
	    }
	}
		
	void QtSinglePlayer::pause()
	{
	    if(_player) _player->Pause();
	}
		
	void QtSinglePlayer::stop()
	{
	    if(_player) _player->Stop();
	}
		
   
	void QtSinglePlayer::SetPlaySegment(const MediaTime& time)
	{
	    if(_player) _player->SetBounds(time);
	}

	bool QtSinglePlayer::IsPlaying()
	{
	    if(_player) return _player->IsPlaying();
	    return false;
	}
		
    }
}

// END
