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

#include <qpainter.h>
#include <qtooltip.h>
#include "VZLabel.hxx"
namespace CLAM
{
	namespace VM
	{
		VZLabel::VZLabel(QWidget* parent) : QFrame(parent)
		{
			_align = AlignCenter;
			InitLabel();
		} 

		VZLabel::~VZLabel()
		{
		}

		void VZLabel::InitLabel()
		{
			_f.setFamily("Sans");
			_f.setPointSize(8);
			_f.setBold(true);

			this->setMinimumSize(20,25);
			this->setFixedWidth(20);
			this->setMaximumHeight(70);
			QToolTip::add(this,"Zoom Ratio");
			this->setText("");
		}
		void VZLabel::drawContents(QPainter* p)
		{
			QRect r = rect();
			p->setFont(_f);
			p->translate(r.x(),r.y()+r.height());
			p->rotate(-90.0);
			p->drawText(0,0,r.height(),r.width(),_align,_text);
			p->resetXForm();
		}

		void VZLabel::setText(const QString& text)
		{
			_text = text;
			int h = _text.length()*_f.pointSize();
			this->setFixedHeight(h);
			update();
		}

	}
}
// END


