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
