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

#ifndef __VMQT_SCROLL_GROUP_H__
#define __VMQT_SCROLL_GROUP_H__

#include <QtGui/QWidget>
#include "vmDataTypes.hxx"

class QLabel;
class QPushButton;
class QScrollBar;

namespace CLAM
{
	namespace VM
	{

		class VerticalLabel;

		class ScrollGroup :  public QWidget
		{
			Q_OBJECT
		public:
			ScrollGroup(EOrientation ori, QWidget* parent=0);
			~ScrollGroup();

			void SetPixmaps(const QPixmap& zin, const QPixmap& zout);
			int GetScrollValue() const;
			int GetMaxScrollValue() const;

		signals:
			void zoomIn();
			void zoomOut();
			void scrollValueChanged(int);

		public:
			void updateScrollValue(int);
			void setMaxScrollValue(int maxValue, int page);
			void updateZoomRatio(QString);

		private:
			QScrollBar*    mScrollBar;
			QLabel*        mHlabel;
			QPushButton*   mZinButton;
			QPushButton*   mZoutButton;

			VerticalLabel* mVlabel;

			void BuildButtons();
			void BuildHorizontal();
			void BuildVertical();
			void MakeConnections();
		};
	}
}

#endif

