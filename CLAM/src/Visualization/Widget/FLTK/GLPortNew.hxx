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
#include "FLTKWrapper.hxx"


namespace CLAMGUI
{

class GLPort : public Fl_Gl_Window
{

	class DrawCallbackAdapter
	{
		
		CBL::Functor0    mDrawCb;
		bool             mValid;
		public:
		DrawCallbackAdapter()
			: mValid( false )
		{
		}

		inline void SetCallback( const CBL::Functor0& cb )
		{
			mDrawCb = cb;
			mValid = true;
		}

		inline void operator()()
		{
			if ( mValid )
				mDrawCb();
		}

	};

	class CullingCallbackAdapter
	{
		
		CBL::Functor4<float, float, unsigned,unsigned>    mCullingCb;
		bool                                     mValid;
		public:
		CullingCallbackAdapter()
			: mValid( false )
		{
		}

		inline void SetCallback( const CBL::Functor4<float,float,unsigned,unsigned>& cb )
		{
			mCullingCb = cb;
			mValid = true;
		}

		inline void operator()( float left, float right, unsigned pixel_width,unsigned pixel_height)
		{
			if ( mValid )
				mCullingCb( left, right, pixel_width, pixel_height );
		}

	};


public:

	Range mVerRange;
	Range mHorRange;

public:

	GLPort( const Rect<int>& geometry, const char* label = 0 ) :
		Fl_Gl_Window( geometry.GetX(), geometry.GetY(), geometry.GetW(), geometry.GetH(), label ),
		mIsConf( false ), mTimerLaunched( false )
	{

	}

	~GLPort()
	{
		FLTKWrapper& tk = dynamic_cast<FLTKWrapper&>(WidgetTKWrapper::GetWrapperFor("FLTK" ));
		
		tk.CancelAsynchronousRefresh( mRefreshSlot );
		
		delete mRenderingState;

	}

	void Configure( GLState* state, const CBL::Functor0& draw_cb )
	{
		mIsConf = false;
		mRenderingState = state;
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

	inline void SetDrawingCallback( const CBL::Functor0& cb )
	{
		mDrawCb.SetCallback( cb );
	}

	inline void SetCullingCallback( const CBL::Functor4<float,float,unsigned,unsigned>& cb )
	{
		mCullCb.SetCallback( cb );
	}

	void DrawSelf(); // Kludge

	void draw(); // Fl_Gl_Window required interface

	int handle( int ); // Fl_Gl_Window required interface

	void damage()
	{
		invalidate();
		Fl_Gl_Window::damage();
	}

protected:

	void ApplyProjection();
	
private:

	DrawCallbackAdapter     mDrawCb;
	CullingCallbackAdapter  mCullCb;
	bool                    mIsConf;
	bool                    mTimerLaunched;
	GLState*                mRenderingState;
	unsigned                mRefreshSlot;
};

}

#endif // GLPort_new.hxx


