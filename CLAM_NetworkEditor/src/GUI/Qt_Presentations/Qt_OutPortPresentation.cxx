/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Qt_OutPortPresentation.hxx"
#include <qpainter.h>

#include <qtooltip.h>
namespace NetworkGUI
{

Qt_OutPortPresentation::Qt_OutPortPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ), mDown(false), mId(id)
{
	updatePosition();
	setFixedSize(10,5);
}

void Qt_OutPortPresentation::SetName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_OutPortPresentation::Show()
{
	show();
}

void Qt_OutPortPresentation::Hide()
{
	hide();
}

void Qt_OutPortPresentation::paintEvent( QPaintEvent * )
{
	QColor c(40, 40, 200);
	QPainter p( this );
        p.setBrush( c );
	p.drawRoundRect( 0,0, width(),height()); // draw a rectangle
}

void Qt_OutPortPresentation::mousePressEvent( QMouseEvent *m)
{
	SignalAcquireOutPortClicked.Emit(this);
}

void Qt_OutPortPresentation::updatePosition()
{
	move(parentWidget()->width()-10,mId*7+7);
}

} // namespace NetworkGUI

