#ifndef __FL_GL_2DSURFACE__
#define __FL_GL_2DSURFACE__

#include "DrawingSurface2D.hxx"
#include <FL/Fl_Gl_Window.H>

namespace CLAMVM
{

	class Fl_Gl_2DSurface : public DrawingSurface2D, public Fl_Gl_Window
	{
	public:
		Fl_Gl_2DSurface( int X, int Y, int W, int H, const char* label = 0 );

		~Fl_Gl_2DSurface();

		virtual void draw();
		virtual void draw_overlay();
		virtual void QueryScreenBoundBox( ScreenBoundBox& sbbox );
		void ResizeGL( );
		void InitGL();

		void EnableIndexedMode();
		void DisableIndexedMode();
		bool IsIndexedModeEnabled() const;
		void EnableDoubleBuffering();
		void DisableDoubleBuffering();
		bool IsDoubleBufferingEnabled() const;

		virtual void RedrawOverlay();

	protected:
		virtual void WCSProject();
		virtual void WindowProject();
		virtual void Refresh();
		virtual void resize( int X, int Y, int W, int H );


	private:
		bool        mTimerLaunched;
		unsigned    mRefreshSlot;
	};

}

#endif // Fl_Gl_2DSurface.hxx
