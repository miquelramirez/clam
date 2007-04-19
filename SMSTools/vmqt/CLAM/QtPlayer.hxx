/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __QTPLAYER__
#define __QTPLAYER__

#include <qwidget.h>
#include <CLAM/MediaTime.hxx>
#include <CLAM/Slotv1.hxx>

using SigSlot::Slotv1;

class QBoxLayout;
class QFrame;
class QPushButton;

namespace CLAM
{
    namespace VM
    {	
		enum ColorMap { WindowMode=0, BlackBackground, WhiteBackground };

		class QtPlayer : public QWidget
		{
			Q_OBJECT
			
		public:
			QtPlayer(QWidget* parent=0);
			virtual ~QtPlayer();

			virtual void SetPlaySegment(const MediaTime& time)=0;
			virtual bool IsPlaying()=0;
			virtual bool IsRunning()=0;

			virtual void SetColorMap(ColorMap map);

			virtual void SetSlotPlayingTime(Slotv1<TData>& slot)=0;
			virtual void SetSlotStopPlaying(Slotv1<TData>& slot)=0;
				
        public slots:
			virtual void play()=0;
			virtual void pause()=0;
			virtual void stop()=0;

		protected:
			QBoxLayout* layout;
			QColor winBackground;

			enum { PAUSE=0, STOP=1 };

			virtual void WindowModeCM();
			virtual void BlackBackgroundCM();
			virtual void WhiteBackgroundCM();

			void RemoveButton(int id);

		private:
			QFrame* container;
			QPushButton *mPlay, *mPause, *mStop;

			void InitWidgets();
		
		};
    }
}

#endif

