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

#ifndef __FL_SINTRACKS__
#define __FL_SINTRACKS__

#include <FL/Fl_Window.H>
#include "SinTracksPresentation.hxx"
#include "SinTracksRenderingManager.hxx"
#include "SineTracksDef.hxx"

namespace CLAMVM
{

	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_GridLayout;
	class Fl_Gl_Single_Display;

	class Fl_SinTracks : public SinTracksPresentation, public Fl_Window
	{
	private:
		Fl_GridLayout*            mLayout;
		Fl_X_Axis*                mXAxis;
		Fl_Y_Axis*                mYAxis;
		Fl_ZoomSlider*            mXSlider;
		Fl_ZoomSlider*            mYSlider;
		Fl_Gl_Single_Display*     mDisplay;
		SinTracksRM               mDrawMgr;

	protected:
		virtual void OnNewTrackList( SineTrackList& list, TSize framelen ); 
				
		virtual void OnNewRange( TData );

		virtual void OnNewDuration( TTime begin, TTime end );

	public:

		Fl_SinTracks( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SinTracks();

		virtual void resize( int X, int Y, int W, int H );
		virtual void Show();
		virtual void Hide();

	};

}

#endif // Fl_SinTracks.hxx
