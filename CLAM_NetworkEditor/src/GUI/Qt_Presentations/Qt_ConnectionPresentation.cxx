
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
//	setAutoMask(true);
	SetOutPos.Wrap( this, &Qt_ConnectionPresentation::OnNewOutPos);
	SetInPos.Wrap( this, &Qt_ConnectionPresentation::OnNewInPos);
	setFixedSize(0,0);

	mPositions[0] = QPoint(0,0);
	mPositions[1] = QPoint(0,0);
	mPositions[2] = QPoint(0,0);
	mPositions[3] = QPoint(0,0);
}

void Qt_ConnectionPresentation::setAutoMask(bool b)
{
/*	if (b) 
		setBackgroundMode( PaletteForeground );
	else 
		setBackgroundMode( PaletteBackground );
	QWidget::setAutoMask(b);
*/
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
//	updateMask();

}

void Qt_ConnectionPresentation::paintEvent( QPaintEvent * e)
{ 


	QBitmap bm( size() );
	bm.fill( color0 );			//transparent

	QPainter paint;
	paint.begin( &bm, this );
	paint.setPen( QPen(color1, 3));

	UpdateBezierLine();
	paint.drawCubicBezier(mPositions);
	paint.end();

    
	QPixmap offscreen(size());
	offscreen.fill(this, QPoint(0,0));


	QPainter p( &offscreen );

	p.setPen( QPen ( QColor( 240, 200, 200 ), 3));
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

void Qt_ConnectionPresentation::updateMask()	// paint clock mask
{
/*
	QBitmap bm( size() );
	bm.fill( color0 );			//transparent

	QPainter paint;
	paint.begin( &bm, this );
//	paint.setBrush( color1 );		// use non-transparent color
	paint.setPen( QPen(color1, 3));

	UpdateBezierLine();
	paint.drawCubicBezier(mPositions);
	paint.end();
	setMask( bm );
*/
}

void Qt_ConnectionPresentation::UpdateBezierLine()
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
		RemoveConnection.Emit( mOutName, mInName, this );
		Hide();
		mDown = false;
		releaseKeyboard();
		break;
	}
}

} // namespace NetworkGUI

