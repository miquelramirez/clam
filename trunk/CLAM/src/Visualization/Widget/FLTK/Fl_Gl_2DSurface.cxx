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

#include "Fl_Gl_2DSurface.hxx"
#include "CLAMGL.hxx"
#include "FLTKWrapper.hxx"
#include "Assert.hxx"
#include <cstring>
#include "CLAM_Math.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <iostream>
#include <FL/gl.h>

namespace CLAMVM
{
	Fl_Gl_2DSurface::Fl_Gl_2DSurface( int X, int Y, int W, int H, const char* label )
		: Fl_Gl_Window( X, Y, W, H, label ), mTimerLaunched( false )
	{
	}

	Fl_Gl_2DSurface::~Fl_Gl_2DSurface()
	{
		FLTKWrapper& tk = dynamic_cast<FLTKWrapper&>(WidgetTKWrapper::GetWrapperFor("FLTK" ));
		
		tk.CancelAsynchronousRefresh( mRefreshSlot );				
	}
	
	void Fl_Gl_2DSurface::draw( )
	{
		if ( !valid() )
			ResizeGL();

		glClear( GL_COLOR_BUFFER_BIT );
		DrawContents();
		redraw_overlay();
		
		glFlush();

		if ( !mTimerLaunched )
		{
			FLTKWrapper& tk = dynamic_cast<FLTKWrapper&>( WidgetTKWrapper::GetWrapperFor("FLTK") );
			mRefreshSlot = tk.RequestAsynchronousRefresh( this );

			mTimerLaunched = true;
		}

	}

	void Fl_Gl_2DSurface::QueryScreenBoundBox( ScreenBoundBox& sbbox )
	{
		sbbox.pixel_left = x();
		sbbox.pixel_top = y();
		sbbox.pixel_width = w();
		sbbox.pixel_height = h();
	}

	void Fl_Gl_2DSurface::RedrawOverlay()
	{
		redraw_overlay();
	}

	void Fl_Gl_2DSurface::draw_overlay()
	{
		if ( !TooltipText() )
			return;
		WindowProject();
		gl_font( FL_HELVETICA, 10 );
		int width = (int)gl_width( TooltipText() );
		int height = (int)gl_height();
		gl_color( FL_BLACK );
		glEnable( GL_SCISSOR_TEST );
		glScissor( w() - width - 4, h() - height - 2, width +2 , height + 2 );
		gl_rectf( w() - width - 4, h() - height - 2, width, height + 2 );
		gl_color( FL_WHITE );
		gl_draw( TooltipText(), w() - width - 2, h() - height );
		glDisable( GL_SCISSOR_TEST );
		WCSProject();
	}

	void Fl_Gl_2DSurface::Refresh()
	{
		redraw();
	}

	void Fl_Gl_2DSurface::resize( int X, int Y, int W, int H )
	{
		Fl_Gl_Window::resize(X,Y,W,H);
		invalidate();
		Fl_Gl_Window::damage();
	}

	void Fl_Gl_2DSurface::ResizeGL( )
	{
		glMatrixMode( GL_PROJECTION );
		glViewport( 0, 0, w(), h() );
		glMatrixMode( GL_MODELVIEW );

		DamageProjection();
	}

	void Fl_Gl_2DSurface::InitGL( )
	{		
		mode( FL_RGB | FL_SINGLE );				
	}

	void Fl_Gl_2DSurface::EnableIndexedMode()
	{
		CLAM_ASSERT( can_do( mode()|FL_INDEX), "OpenGL driver doesn't support Indexed Mode!");
		mode( mode()|FL_INDEX );
	}

	void Fl_Gl_2DSurface::DisableIndexedMode()
	{
		mode( mode()&(~FL_INDEX));
	}

	bool Fl_Gl_2DSurface::IsIndexedModeEnabled() const
	{
		return mode()&FL_INDEX;
	}

	void Fl_Gl_2DSurface::EnableDoubleBuffering()
	{
		CLAM_ASSERT( can_do( mode()|FL_DOUBLE), "OpenGL driver doesn't support Indexed Mode!");
		mode( mode()|FL_DOUBLE );
	}
		
	void Fl_Gl_2DSurface::DisableDoubleBuffering()
	{
		mode( mode()&(~FL_DOUBLE));
	}
		
	bool Fl_Gl_2DSurface::IsDoubleBufferingEnabled() const
	{
		return mode()&FL_DOUBLE!=0;
	}

	void Fl_Gl_2DSurface::WCSProject()
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( mcxmin, mcxmax, mcymin, mcymax, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
	}

	void Fl_Gl_2DSurface::WindowProject()
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, w(), 0, h(), -1, 1 );
		glMatrixMode( GL_MODELVIEW );
	}
}
