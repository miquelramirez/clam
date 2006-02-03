#include <QHBoxLayout>
#include <QPushButton>
#include "vm_player.hxx"
#include "vm_wplayer.hxx"

namespace CLAM
{
	namespace VM
	{
		WPlayer::WPlayer(QWidget* parent)
			: QWidget(parent) 
			, wp_player(0)
		{
			build_wplayer();
		}
			
		WPlayer::~WPlayer()
		{
			if(wp_player) delete wp_player;
		}

		void WPlayer::set_player(Player* player)
		{
			wp_player = player;
		}

		void WPlayer::set_pixmaps(const QPixmap& play, const QPixmap& pause, const QPixmap& stop)
		{
			wp_play->setIcon(QIcon(play));
			wp_pause->setIcon(QIcon(pause));
			wp_stop->setIcon(QIcon(stop));
		}

		void WPlayer::play()
		{
			if(!wp_player) return;
			wp_player->play();
		}

		void WPlayer::pause()
		{
			if(!wp_player) return;
			wp_player->pause();
		}

		void WPlayer::stop()
		{
			if(!wp_player) return;
			wp_player->stop();
		}

		void WPlayer::build_wplayer()
		{		
			wp_play = new QPushButton(this);
			wp_play->setFixedSize(25,20);
			wp_play->setToolTip("Play");

			wp_pause = new QPushButton(this);
			wp_pause->setFixedSize(25,20);
			wp_pause->setToolTip("Pause");

			wp_stop = new QPushButton(this);
			wp_stop->setFixedSize(25,20);
			wp_stop->setToolTip("Stop");

			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);

			layout->addWidget(wp_play);
			layout->addWidget(wp_pause);
			layout->addWidget(wp_stop);

			// Connections
			connect(wp_play,SIGNAL(clicked()),this,SLOT(play()));
			connect(wp_pause,SIGNAL(clicked()),this,SLOT(pause()));
			connect(wp_stop,SIGNAL(clicked()),this,SLOT(stop()));
		}
	}
}

// END
