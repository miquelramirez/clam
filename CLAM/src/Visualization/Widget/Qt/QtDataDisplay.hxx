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

#ifndef __QtDataDisplay__
#define __QtDataDisplay__

#include <qmainwindow.h>
#include "Viewport.hxx"
class QSizePolicy;
class QScrollBar;
class QSlider;
class QwtScale;

namespace CLAMGUI
{

class QtGLPort;

class QtDataDisplay : public QWidget
{
	Q_OBJECT
public:
	QtDataDisplay( QtGLPort* display, QWidget* parent = 0, const char* name = 0  );

	QtDataDisplay( QWidget* parent = 0, const char* name = 0 );

	virtual ~QtDataDisplay();

	QSize sizeHint();
	
	QSize minimumSize();

	QSizePolicy sizePolicy();

public slots:

	void XAxisZoom( int value );

	void YAxisZoom( int value );

	void XAxisDisp( int value );

	void YAxisDisp( int value );

	void SetupDisplay();

signals:

	void RefreshPlot( const Viewport& );

public:
	 
	virtual void SetupContents( QtGLPort* );

protected:

	class ViewState
	{
	public:
		ViewState( );

		Viewport UpdateXZoom( int value, const Viewport& actualview );

		Viewport UpdateYZoom( int value, const Viewport& actualview );

		Viewport UpdateXScroll( int value, const Viewport& actualview );

		Viewport UpdateYScroll( int value, const Viewport& actualview );

		void SetView( const Viewport& v );

		int GetCurrentXSteps() const;

		int GetCurrentYSteps() const;

		void ResetDisplacement();

	private:
		int    mPrevXf;
		int    mPrevYf;
		float  original_width;
		float  original_height;
		float  actual_width;
		float  actual_height;
		int    currentXStepping;
		int    currentYStepping;

		Viewport mView;

	};


private:
	
	QtGLPort*        mDataPort;
	Viewport*        mView;
	QScrollBar*      mXDisp;
	QScrollBar*      mYDisp;
	QSlider*         mXZoom;
	QSlider*         mYZoom;
	QwtScale*        mXRuler;
	QwtScale*        mYRuler;

	ViewState        mCurrentZoom;


};

}

#endif //QtDataDisplay.hxx

