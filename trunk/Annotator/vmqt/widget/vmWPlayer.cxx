/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
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

