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

#include "Qt_PortConnectionPresentation.hxx"
#include <qpixmap.h>
#include <iostream>
#include <qpainter.h>
#include <qbitmap.h>


namespace NetworkGUI
{

Qt_PortConnectionPresentation::Qt_PortConnectionPresentation( QWidget *parent, const char *name)
	: Qt_ConnectionPresentation( parent, name )
{
}

Qt_PortConnectionPresentation::~Qt_PortConnectionPresentation()
{
}

void Qt_PortConnectionPresentation::paintEvent( QPaintEvent * e)
{ 


	QBitmap bm( size() );
	bm.fill( color0 );			//transparent

	QPainter paint;
	paint.begin( &bm, this );
	paint.setPen( QPen(color1, 5));

	UpdateBezierLine();
	paint.drawCubicBezier(mPositions);
	paint.end();

    
	QPixmap offscreen(size());
	offscreen.fill(this, QPoint(0,0));


	QPainter p( &offscreen );

	p.setPen( QPen ( QColor( 240, 200, 200 ), 5));
	p.drawCubicBezier(mPositions);
	if (mDown)
		p.setPen( QPen ( QColor( 200, 50, 50), 1 ) );
	else
		p.setPen( QPen ( QColor( 80, 80, 200), 1 ) );
	p.drawCubicBezier(mPositions);
	p.flush();
        p.end();
	setMask( bm );	
        bitBlt(this, 0, 0, &offscreen);
}

void Qt_PortConnectionPresentation::UpdateBezierLine()
{

	if(((origin.x()<end.x()) && origin.y()<end.y()) || 
	   ((origin.x()>end.x()) && origin.y()>end.y()))  // origin is in 1 o 3 quadrant of end
	{
		mPositions.setPoint(0, QPoint(mapFromParent( geometry().topLeft() )));
		mPositions.setPoint(1, QPoint(mapFromParent(geometry().topRight() )));
		mPositions.setPoint(2, QPoint(mapFromParent(geometry().bottomLeft() )));
		mPositions.setPoint(3, QPoint(mapFromParent(geometry().bottomRight() )));
	}
	else
	{	
		mPositions.setPoint(0, QPoint(mapFromParent(geometry().bottomLeft() )));
		mPositions.setPoint(1, QPoint(mapFromParent(geometry().bottomRight() )));	
		mPositions.setPoint(2, QPoint(mapFromParent( geometry().topLeft() )));
		mPositions.setPoint(3, QPoint(mapFromParent(geometry().topRight() )));
	}
}

} // namespace NetworkGUI
