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

#include "QtDataDisplay.hxx"
#include "QtGLPort.hxx"
#include <qwidget.h>
#include <qsizepolicy.h>
#include <qslider.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qwt_scale.h>
#include <qwt_scldraw.h>
#include <qwt_scldiv.h>
#include "Viewport.hxx"

#include <cmath>

using namespace CLAMGUI;

QtDataDisplay::QtDataDisplay( QtGLPort* display, QWidget* parent, const char* name )
	: QWidget( parent, name ), mDataPort( display ), mView( NULL )
{
	setMinimumSize( 320, 240);
}

QtDataDisplay::QtDataDisplay( QWidget* parent, const char* name )
	: QWidget( parent, name ), mView( NULL )
{
	setMinimumSize( 640, 480 );
}

QtDataDisplay::~QtDataDisplay()
{
}

QSize QtDataDisplay::sizeHint()
{
	return QSize( 640, 480 );
}

QSize QtDataDisplay::minimumSize()
{
	return QSize( 320, 240 );
}

QSizePolicy QtDataDisplay::sizePolicy()
{
	return QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

void QtDataDisplay::SetupContents( QtGLPort* port )
{
	mDataPort = port;

	QGridLayout* grid = new QGridLayout( this, 10, 15);
	
	mXDisp = new QScrollBar( QScrollBar::Horizontal, this, "XaxisDisp" );
	mYDisp = new QScrollBar( QScrollBar::Vertical, this, "YaxisDisp" );
	mXZoom = new QSlider( QScrollBar::Horizontal, this, "XaxisZoom" );
	mYZoom = new QSlider( QScrollBar::Vertical, this, "YaxisZoom" );
	mXRuler = new QwtScale( QwtScale::Bottom, this, "XaxisScale" );
	mYRuler = new QwtScale( QwtScale::Right, this, "YaxisScale" );

	// Scrollbars stepping
	mXZoom->setSteps( 1, 1 );
	mYZoom->setSteps( 1, 1 );

	mXDisp->setSteps( 1, 1 );
	mYDisp->setSteps( 1, 1 );


	SetupDisplay();

	grid->addMultiCellWidget( mDataPort, 0, 6, 0, 10 );
	grid->addMultiCellWidget( mXRuler, 7, 7, 0, 10 );
	grid->addMultiCellWidget( mXDisp, 8, 8, 0, 10 );
	grid->addMultiCellWidget( mXZoom, 9, 9, 0, 10 );

	grid->addMultiCellWidget( mYRuler, 0, 6, 11, 12 );
	grid->addMultiCellWidget( mYDisp, 0, 6, 13, 13 );
	grid->addMultiCellWidget( mYZoom, 0, 6, 14, 14 );

	// Signal/Slot connection

	connect( mXDisp, SIGNAL(valueChanged(int)), this, SLOT(XAxisDisp(int)) );
	connect( mYDisp, SIGNAL(valueChanged(int)), this, SLOT(YAxisDisp(int)) );
	connect( mXZoom, SIGNAL(valueChanged(int)), this, SLOT(XAxisZoom(int)) );
	connect( mYZoom, SIGNAL(valueChanged(int)), this, SLOT(YAxisZoom(int)) );
	connect( this, SIGNAL(RefreshPlot(const Viewport&)), mDataPort, SLOT(ViewportChanged(const Viewport&)) );
	connect( mDataPort, SIGNAL(ViewportReceived()), this, SLOT(SetupDisplay()) );

}

void QtDataDisplay::SetupDisplay()
{
Viewport v = mDataPort->GetView();
	
	mCurrentZoom.SetView( v );
		
	// horizontal slider setup
	mXDisp->setMinValue(0);
	mXDisp->setMaxValue(0);
	mXDisp->setValue(0);
	mXDisp->setTracking(false);
	mXZoom->setMinValue(1);
	mXZoom->setMaxValue(20);
	mXZoom->setTracking(false);
	QwtScaleDiv dummydiv;
	dummydiv.rebuild( v.left, v.right, 3, 10, false );
	mXRuler->setScaleDiv( dummydiv );
	mXRuler->setScaleDraw( new QwtScaleDraw );
	// vertical slider setup
	mYDisp->setMinValue(0);
	mYDisp->setMaxValue(0);
	mYDisp->setValue(0);
	mYZoom->setMinValue(1);
	mYZoom->setMaxValue(20);
	mYZoom->setValue(20);
	mYDisp->setTracking(false);
	mYZoom->setTracking(false);

	dummydiv.rebuild( v.bottom, v.top, 3, 10, false );
	mYRuler->setScaleDiv( dummydiv );
	mYRuler->setScaleDraw( new QwtScaleDraw );
}

void QtDataDisplay::XAxisDisp( int value )
{

	Viewport v = mCurrentZoom.UpdateXScroll( value, mDataPort->GetView() );

	QwtScaleDiv dummy;

	QwtScaleDiv dummydiv;
	dummydiv.rebuild( v.left, v.right, 3, 10, false );

	mXRuler->setScaleDiv( dummydiv );

	emit RefreshPlot( v );

}

void QtDataDisplay::YAxisDisp( int value )
{
	Viewport v = mCurrentZoom.UpdateYScroll( value, mDataPort->GetView() );

	QwtScaleDiv dummy;

	QwtScaleDiv dummydiv;
	dummydiv.rebuild( v.bottom, v.top, 3, 10, false );

	mYRuler->setScaleDiv( dummydiv );

	emit RefreshPlot( v );
}

void QtDataDisplay::XAxisZoom( int value )
{
	mXDisp->setValue(0);

	Viewport v = mCurrentZoom.UpdateXZoom( value, mDataPort->GetView() );

	QwtScaleDiv dummydiv;
	dummydiv.rebuild( v.left, v.right, 3, 10, false );

	mXDisp->setMaxValue( mCurrentZoom.GetCurrentXSteps() );

	mXRuler->setScaleDiv( dummydiv );

	emit RefreshPlot(v);
}

void QtDataDisplay::YAxisZoom( int value )
{
	mYDisp->setValue(0);

	Viewport v = mCurrentZoom.UpdateYZoom( value, mDataPort->GetView() );

	QwtScaleDiv dummydiv;
	dummydiv.rebuild( v.bottom, v.top, 3, 10, false );

	mYDisp->setMaxValue( mCurrentZoom.GetCurrentYSteps() );		
	
	mYRuler->setScaleDiv( dummydiv );

	emit RefreshPlot(v);
}

QtDataDisplay::ViewState::ViewState()
	: mPrevXf(0), mPrevYf(0), original_height(0), original_width(0), actual_height(0), actual_width(0),
	currentXStepping(0), currentYStepping(0)
{
}

void QtDataDisplay::ViewState::SetView( const Viewport& v)
{
	mView = v;

	original_height = actual_height = fabs( mView.top - mView.bottom );
	original_width = actual_width = fabs( mView.right - mView.left );
	currentXStepping = currentYStepping = 0;
	mPrevXf = mPrevYf = 0;
}

Viewport QtDataDisplay::ViewState::UpdateXZoom( int value, const Viewport& actual )
{
	Viewport newView = mView;

	float factor = 1.0f / (float)value;

	newView.top = actual.top;
	newView.bottom = actual.bottom;
	newView.left *=factor;
	newView.right *=factor;

	actual_width = fabs( newView.right - newView.left );
	currentXStepping = ( ceil( original_width / actual_width ) )-1;

	return newView;
}

Viewport QtDataDisplay::ViewState::UpdateYZoom( int value, const Viewport& actual )
{

	Viewport newView = mView;

	float factor = 1.0f / (float)(21 - value);

	newView.top *=factor;
	newView.bottom *=factor;
	newView.left = actual.left;
	newView.right = actual.right;

	actual_height = fabs( newView.top - newView.bottom );
	currentYStepping = ( ceil( original_height / actual_height )) -1;

	return newView;
}

Viewport QtDataDisplay::ViewState::UpdateXScroll( int value,  const Viewport& actual )
{
	Viewport newView = mView;

	newView.top = actual.top;
	newView.bottom = actual.bottom;
	newView.left = actual.left + (actual_width * ( value - mPrevXf));
	newView.right =  actual.right + (actual_width * (value - mPrevXf) );

	mPrevXf = value;

	return newView;
}

Viewport QtDataDisplay::ViewState::UpdateYScroll( int value, const Viewport& actual )
{
	Viewport newView = mView;

	newView.left = actual.left;
	newView.right = actual.right;
	newView.top = actual.top +  (actual_height * ( value - mPrevYf));
	newView.bottom = actual.bottom + (actual_height * ( value - mPrevYf) );

	mPrevYf = value;

	return newView;

}

int QtDataDisplay::ViewState::GetCurrentXSteps() const
{
	return currentXStepping;
}

int QtDataDisplay::ViewState::GetCurrentYSteps() const
{
	return currentYStepping;
}

void QtDataDisplay::ViewState::ResetDisplacement()
{
	mPrevXf = mPrevYf = 0;
}

