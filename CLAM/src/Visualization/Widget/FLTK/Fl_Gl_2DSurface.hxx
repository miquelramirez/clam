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
		virtual void resize( int X, int Y, int W, int H );

	protected:
		virtual void WCSProject();
		virtual void WindowProject();
		virtual void Refresh();


	private:
		bool        mTimerLaunched;
		unsigned    mRefreshSlot;
	};

}

#endif // Fl_Gl_2DSurface.hxx
