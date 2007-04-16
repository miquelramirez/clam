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

#ifndef __QTGLPORT__
#define __QTGLPORT__

#include <qgl.h>
#include "CBL.hxx"
#include "Range.hxx"
#include "Viewport.hxx"

namespace CLAMGUI
{
class GLState;

class Range;

class QtGLPort : public QGLWidget
{
	Q_OBJECT
public:

	QtGLPort( QWidget* parent, const char* name );

	virtual ~QtGLPort();

	void Configure( GLState* state, const CBL::Functor0& drawcb );

	void SetView( const Viewport& v );

	const Viewport& GetView() const;

	QSize minimumSize();

	QSize sizeHint();

public slots:

	void ViewportChanged( const Viewport& v );

signals:

	void ViewportReceived();
protected:

	void initializeGL();

	void resizeGL( int w, int h );

	void paintGL();

private:
	
	CBL::Functor0      mDrawCb;
    bool               mIsConf;
	GLState*           mState;
	Viewport           mView;
};

}

#endif // QtGLPort.hxx

