#include <qframe.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include "IconData.hxx"
#include "Player.hxx"
#include "PlayableList.hxx"
#include "QtPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		QtPlayer::QtPlayer(QWidget* parent) : QWidget(parent)
		{
			_player = NULL;
			InitWidgets();
		}
		
		QtPlayer::~QtPlayer()
		{
			if(_player) delete _player;
		}
		
		void QtPlayer::SetPlayer(Player* player)
		{
			_player = player;
		}

		void QtPlayer::play()
		{
			if(_player)
			{
				PlayableList::Stop();
				_player->Play();
			}
		}
		
		void QtPlayer::pause()
		{
			if(_player) _player->Pause();
		}
		
		void QtPlayer::stop()
		{
			if(_player) _player->Stop();
		}
		
		void QtPlayer::InitWidgets()
		{
			QPixmap icon_play((const char**)icon_play);
			QPixmap icon_pause((const char**)icon_pause);
			QPixmap icon_stop((const char**)icon_stop);

			QFrame* container = new QFrame(this);
			container->setFixedSize(75,35);

			_play = new QPushButton(container);
			_play->setFixedSize(25,20);
			_play->setGeometry(0,5,25,20);
			_play->setPixmap(icon_play);
			QToolTip::add(_play,"Play");

			_pause = new QPushButton(container);
            _pause->setFixedSize(25,20);
			_pause->setGeometry(_play->width(),_play->y(),25,20);
			_pause->setPixmap(icon_pause);
			QToolTip::add(_pause,"Pause");

			_stop = new QPushButton(container);
			_stop->setFixedSize(25,20);
			_stop->setGeometry(_pause->x()+_pause->width(),_play->y(),25,20);
			_stop->setPixmap(icon_stop);
			QToolTip::add(_stop,"Stop");

			// Connections
			connect(_play,SIGNAL(clicked()),this,SLOT(play()));
			connect(_pause,SIGNAL(clicked()),this,SLOT(pause()));
			connect(_stop,SIGNAL(clicked()),this,SLOT(stop()));
		}

		void QtPlayer::SetPlaySegment(const MediaTime& time)
		{
			if(_player) _player->SetBounds(time);
		}

		bool QtPlayer::IsPlaying()
		{
			if(_player) return _player->IsPlaying();
			return false;
		}
		
	}
}

// END
