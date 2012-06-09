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

#include <CLAM/ScrollBar.hxx>
//Added by qt3to4:
#include <QMouseEvent>

namespace CLAM
{
	namespace VM
	{
		ScrollBar::ScrollBar(Qt::Orientation orientation, QWidget * parent)
			: QScrollBar(orientation,parent)
		{
			setFocusPolicy(Qt::NoFocus);
		}

		ScrollBar::~ScrollBar()
		{
		}

		void ScrollBar::mousePressEvent(QMouseEvent * e)
		{
			QScrollBar::mousePressEvent(e);
			emit clicked();
		}

		void ScrollBar::mouseReleaseEvent(QMouseEvent * e)
		{
			QScrollBar::mouseReleaseEvent(e);
			emit released();
		}
	}
}

// END

