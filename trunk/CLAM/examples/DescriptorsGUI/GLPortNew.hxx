/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __GLPORT__
#define __GLPORT__

#include <FL/Fl_Gl_Window.H>
#include "GeometryKit.hxx"
#include "GLState.hxx"
#include "Range.hxx"
#include "CBL.hxx"
#include "FLMultiDisplay.hxx"

namespace CLAMGUI
{

class GLPort : public Fl_Gl_Window
{
public:

	Range mVerRange;
	Range mHorRange;
	FLMultiDisplay* mInMultiDisplay;

public:

	GLPort( const Rect<int>& geometry, const char* label = 0 ) :
		Fl_Gl_Window( geometry.GetX(), geometry.GetY(), geometry.GetW(), geometry.GetH(), label ),
		mIsConf( false )
	{
		mode( FL_RGB8| FL_DOUBLE );
	}

	~GLPort()
	{
	delete mRenderingState;
	}

	void Configure( GLState* state, const CBL::Functor0& draw_cb )
	{
		mIsConf = false;
		mRenderingState = state;
		mDrawCb = draw_cb;
		mIsConf = true;
	 
	}

	void SetHorRange( const Range& range )
	{
		mHorRange = range;
	}

	void SetVerRange( const Range& range )
	{
		mVerRange = range;
	}

	void DrawSelf(); // Kludge

	void draw(); // Fl_Gl_Window required interface

	int handle( int ); // Fl_Gl_Window required interface

protected:

	void ApplyProjection();
	
private:
	CBL::Functor0         mDrawCb;
	bool                  mIsConf;
	GLState*         mRenderingState;
};

}

#endif // GLPort_new.hxx

