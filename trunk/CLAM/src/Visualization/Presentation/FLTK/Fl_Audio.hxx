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

#ifndef __FL_AUDIO__
#define __FL_AUDIO__

#include <FL/Fl_Window.H>
#include "AudioPresentation.hxx"
#include "AudioRenderingManager.hxx"

namespace CLAMVM
{
	// forward declarations
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_Gl_Single_Display;
	class Fl_GridLayout;
	
	class Fl_Audio : public AudioPresentation, public Fl_Window
	{
		
	private:
		Fl_X_Axis*              mXAxis;
		Fl_Y_Axis*              mYAxis;
		Fl_ZoomSlider*          mXSlider;
		Fl_ZoomSlider*          mYSlider;
		Fl_GridLayout*          mLayout;
		Fl_Gl_Single_Display*   mDisplay;
		AudioRenderingManager   mDrawMgr;
		
	protected:
		
		virtual void OnNewAudio( const DataArray&, TTime, TTime, TData );
		
	public:
		Fl_Audio( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_Audio();
		
		void resize( int X, int Y, int W, int H );
		void Show();
		void Hide();
		

	};

}


#endif // Fl_Audio.hxx
