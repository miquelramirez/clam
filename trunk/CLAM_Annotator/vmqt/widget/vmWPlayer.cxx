#include <QHBoxLayout>
#include <QPushButton>
#include "vmPlayer.hxx"
#include "vmWPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		WPlayer::WPlayer(QWidget* parent)
			: QWidget(parent) 
			, mPlayer(0)
		{
			BuildWPlayer();
		}
			
		WPlayer::~WPlayer()
		{
			if(mPlayer) delete mPlayer;
		}

		void WPlayer::SetPlayer(Player* player)
		{
			mPlayer = player;
		}

		void WPlayer::SetPixmaps(const QPixmap& play, const QPixmap& pause, const QPixmap& stop)
		{
			mPlay->setIcon(QIcon(play));
			mPause->setIcon(QIcon(pause));
			mStop->setIcon(QIcon(stop));
		}

		void WPlayer::play()
		{
			if(!mPlayer) return;
			mPlayer->play();
		}

		void WPlayer::pause()
		{
			if(!mPlayer) return;
			mPlayer->pause();
		}

		void WPlayer::stop()
		{
			if(!mPlayer) return;
			mPlayer->stop();
		}

		void WPlayer::BuildWPlayer()
		{		
			mPlay = new QPushButton(this);
			mPlay->setFixedSize(25,20);
			mPlay->setToolTip("Play");

			mPause = new QPushButton(this);
			mPause->setFixedSize(25,20);
			mPause->setToolTip("Pause");

			mStop = new QPushButton(this);
			mStop->setFixedSize(25,20);
			mStop->setToolTip("Stop");

			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);

			layout->addWidget(mPlay);
			layout->addWidget(mPause);
			layout->addWidget(mStop);

			// Connections
			connect(mPlay,SIGNAL(clicked()),this,SLOT(play()));
			connect(mPause,SIGNAL(clicked()),this,SLOT(pause()));
			connect(mStop,SIGNAL(clicked()),this,SLOT(stop()));
		}
	}
}

// END
