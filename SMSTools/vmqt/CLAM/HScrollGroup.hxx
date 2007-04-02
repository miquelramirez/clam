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

#ifndef __HSCROLLGROUP__
#define __HSCROLLGROUP__

#include <qwidget.h>

class QLabel;
class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class ScrollBar;

		class HScrollGroup : public QWidget
		{
			Q_OBJECT

		public:
			HScrollGroup(QWidget* parent=0);
			~HScrollGroup();

			int GetScrollValue() const;
			int GetMaxScrollValue() const;
			
	    signals:
			void zoomIn();
			void zoomOut();
			void scrollValueChanged(int);
			void zoomRatio(double);
			void maxScrollValue(int);
			void updatedScrollValue(int);
			void scrollBarClicked();
			void scrollBarReleased();
			
		public slots:
			void updateZoomRatio(double);
			void updateScrollValue(int);
			void setMaxScrollValue(int);
			
		private:
			ScrollBar*   mScrollBar;
			QLabel*      mLabel;
			QPushButton* mZIn;
			QPushButton* mZOut;

			void Init();
		};
	}
}

#endif

