
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

