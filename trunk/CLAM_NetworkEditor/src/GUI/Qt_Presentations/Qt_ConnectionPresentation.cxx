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

#include "Qt_ConnectionPresentation.hxx"


#include <qpixmap.h>
#include <qpainter.h>
#include <qbitmap.h>

namespace NetworkGUI
{

Qt_ConnectionPresentation::Qt_ConnectionPresentation( QWidget *parent, const char *name)
	: QWidget( parent, name ),
	  origin(-1,-1),
	  end(-1,-1),
	  mPositions(4),
	  mSelected(false)
{
	setPalette( QPalette( QColor( 250, 250, 200) ) );

	SlotSetOutPos.Wrap( this, &Qt_ConnectionPresentation::SetOutPos);
	SlotSetInPos.Wrap( this, &Qt_ConnectionPresentation::SetInPos);
	setFixedSize(0,0);

	mPositions[0] = QPoint(0,0);
	mPositions[1] = QPoint(0,0);
	mPositions[2] = QPoint(0,0);
	mPositions[3] = QPoint(0,0);
}


Qt_ConnectionPresentation::~Qt_ConnectionPresentation()
{
}

void Qt_ConnectionPresentation::SetOutPos(int x, int y)
{
	origin.setX(x);
	origin.setY(y);
	UpdatePosition();
}

void Qt_ConnectionPresentation::SetInPos(int x, int y)
{
	end.setX(x);
	end.setY(y);
	UpdatePosition();
}

void Qt_ConnectionPresentation::Show()
{
	show();
	UpdatePosition();
	repaint();
}

void Qt_ConnectionPresentation::Hide()
{
	hide();
}

void Qt_ConnectionPresentation::ResolveWireZone(int & position, int & extent,
	const int origin, const int end,
	const int wireThickness, const int torsionResistence)
{
	int upperLimit = end-torsionResistence;
	if (upperLimit>origin-wireThickness)
		upperLimit=origin-wireThickness;

	int lowerLimit = origin+torsionResistence;
	if (lowerLimit<end+wireThickness)
		lowerLimit=end+wireThickness;

	position=upperLimit;
	extent=lowerLimit-upperLimit;
}


void Qt_ConnectionPresentation::UpdatePosition()
{
	int x, y, w, h;

	ResolveWireZone(x,w,origin.x(),end.x(),5,60);
	ResolveWireZone(y,h,origin.y(),end.y(),5,60);

	move (x,y);
	setFixedSize(w,h);
}

void Qt_ConnectionPresentation::mouseMoveEvent( QMouseEvent *m)
{
	SignalMovingMouseWithButtonPressed.Emit( m->globalPos() - mPrevPos );
	mPrevPos = m->globalPos();
}



void Qt_ConnectionPresentation::mousePressEvent( QMouseEvent *m)
{
	mPrevPos = m->globalPos();
	if(!mSelected) // already selected
	{
		mSelected = true;
		if((m->button() & LeftButton) && (m->state() & ShiftButton))
			SignalConnectionPresentationAddedToSelection.Emit( this );
		else
			SignalConnectionPresentationSelected.Emit( this );
		repaint();
	}
}

void Qt_ConnectionPresentation::UnSelectConnectionPresentation()
{
	mSelected = false;
	repaint();
}


} // namespace NetworkGUI

