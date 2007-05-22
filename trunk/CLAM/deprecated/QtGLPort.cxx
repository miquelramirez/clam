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

#include "QtGLPort.hxx"
#include "Range.hxx"
#include "GLState.hxx"
#include <qtimer.h>

using namespace CLAMGUI;

QtGLPort::QtGLPort( QWidget* parent, const char* name )
	: QGLWidget( parent, name ), mIsConf( false ), mState( NULL )
{
	setMinimumSize(300,100);

	QTimer* time = new QTimer(this);

	connect( time, SIGNAL(timeout()), this, SLOT(updateGL()) );

	time->start( 166 );

}

QtGLPort::~QtGLPort()
{
	if ( mIsConf )
		delete mState;
}

void QtGLPort::Configure( GLState* state, const CBL::Functor0& drawcb )
{
	if ( mIsConf )
	{
		delete mState;
		mIsConf = false;
	}
	mDrawCb=drawcb;
	mState=state;
	mIsConf = true;
}


void QtGLPort::SetView( const Viewport& v )
{
	mView = v;

	emit ViewportReceived();
}

const Viewport& QtGLPort::GetView() const
{
	return mView;
}

QSize QtGLPort::minimumSize()
{
	return QSize( 100, 100 );
}

QSize QtGLPort::sizeHint()
{
	return QSize( 640, 480 );
}


void QtGLPort::ViewportChanged( const Viewport& v)
{
	mView = v;
	makeCurrent();
	resizeGL( width(), height() );
	glDraw();
}


void QtGLPort::initializeGL( )
{
	if ( mIsConf )
		mState->Apply();
}

void QtGLPort::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
/*	glOrtho( mHorRange.mPosition, mHorRange.mPosition + mHorRange.mSize, 
			 mVerRange.mPosition , mVerRange.mPosition + mVerRange.mSize, 
			 -1.0f, 1.0f );*/
	glOrtho( mView.left, mView.right, mView.bottom, mView.top, -1.0f, 1.0f );
	glMatrixMode( GL_MODELVIEW );
}

void QtGLPort::paintGL( )
{
	if (mIsConf )
		mDrawCb();
}

