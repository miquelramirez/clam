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
#include <iostream>
#include <qpainter.h>
#include <qbitmap.h>

namespace NetworkGUI
{

Qt_ConnectionPresentation::Qt_ConnectionPresentation( QWidget *parent, const char *name)
	: QWidget( parent, name ),
	  origin(-1,-1),
	  end(-1,-1),
	  mPositions(4),
	  mDown(false)
{
	setPalette( QPalette( QColor( 250, 250, 200) ) );

	SetOutPos.Wrap( this, &Qt_ConnectionPresentation::OnNewOutPos);
	SetInPos.Wrap( this, &Qt_ConnectionPresentation::OnNewInPos);
	setFixedSize(0,0);

	mPositions[0] = QPoint(0,0);
	mPositions[1] = QPoint(0,0);
	mPositions[2] = QPoint(0,0);
	mPositions[3] = QPoint(0,0);
}


Qt_ConnectionPresentation::~Qt_ConnectionPresentation()
{
}

void Qt_ConnectionPresentation::OnNewOutPos(int x, int y)
{
	origin.setX(x);
	origin.setY(y);
	UpdatePosition();
}

void Qt_ConnectionPresentation::OnNewInPos(int x, int y)
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

void Qt_ConnectionPresentation::UpdatePosition()
{
	int x, y, w, h;
	if (origin.x() > end.x())
	{
		x = end.x();
		w = origin.x() - end.x();
	}
	else
	{
		x = origin.x();
		w = end.x() - origin.x();
	}

	if (origin.y() > end.y())
	{
		y = end.y();
		h = origin.y() - end.y();
	}
	else
	{
		y = origin.y();
		h = end.y() - origin.y();
	}

	move (x,y);
	setFixedSize(w + 1,h +1);
}


void Qt_ConnectionPresentation::mousePressEvent( QMouseEvent *m)
{
	mDown = true;
	grabKeyboard();
	repaint();
}

void Qt_ConnectionPresentation::mouseReleaseEvent( QMouseEvent *)
{
	mDown = false;
	releaseKeyboard();
	repaint();
}


void Qt_ConnectionPresentation::keyPressEvent( QKeyEvent *k )
{
	switch ( tolower(k->ascii()) ) 
	{
        case 'x': 
		RemoveConnection.Emit( this );
		Hide();
		mDown = false;
		releaseKeyboard();
		break;
	}
}

} // namespace NetworkGUI

