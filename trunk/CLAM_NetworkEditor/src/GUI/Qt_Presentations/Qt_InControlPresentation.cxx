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

#include "Qt_InControlPresentation.hxx"
#include <qpainter.h>
#include <qbitmap.h>
#include <qtooltip.h>

namespace NetworkGUI
{

Qt_InControlPresentation::Qt_InControlPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ),  mId(id)

{	
	move(mId*13+12, 0);
	setFixedSize(10,5);

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() , 232);

	QBitmap bm( size() );
	bm.fill( color0 );			//transparent

	QPainter paint;

	paint.begin( &bm, this );
	paint.setPen( QPen(color1, 1));
	paint.setBrush( QBrush(color1));
	paint.drawPolygon(points);

	QRegion reg(bm);
	mReg += reg;
}

Qt_InControlPresentation::~Qt_InControlPresentation()
{
}

void Qt_InControlPresentation::SetName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_InControlPresentation::Show()
{
	show();
}

void Qt_InControlPresentation::Hide()
{
	hide();
}

void Qt_InControlPresentation::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(200,0,0));

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() , 232);
	p.drawPolygon( points );

}

void Qt_InControlPresentation::mousePressEvent( QMouseEvent *m)
{
	SignalAcquireInControlClicked.Emit(this);
}

QRegion Qt_InControlPresentation::GetRegion()
{
	QRegion reg(mReg);
	reg.translate(pos().x(), pos().y());
	return reg;

}


} // namespace NetworkGUI

