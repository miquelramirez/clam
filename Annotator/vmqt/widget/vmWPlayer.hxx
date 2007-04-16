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

#ifndef __VMQT_WPLAYER_H__
#define __VMQT_WPLAYER_H__

#include <QtGui/QWidget>
#include "vmDataTypes.hxx"

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class Player;
		
		class WPlayer : public QWidget
		{
			Q_OBJECT
		public:
			WPlayer(QWidget* parent=0);
			~WPlayer();

			void SetPlayer(Player* player);
			void SetPixmaps(const QPixmap& play, const QPixmap& pause, const QPixmap& stop);
			
		private slots:
			void play();
			void pause();
			void stop();

		private:
			QPushButton* mPlay;
			QPushButton* mPause;
			QPushButton* mStop;

			Player* mPlayer;

			void BuildWPlayer();
		};
	}
}

#endif

