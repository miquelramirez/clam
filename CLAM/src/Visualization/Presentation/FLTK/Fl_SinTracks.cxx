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

#include "Fl_SinTracks.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include "Fl_Gl_Single_Display.hxx"
#include <algorithm>


namespace CLAMVM
{
	Fl_SinTracks::Fl_SinTracks( int X, int Y, int W, int H, const char* label )
		: Fl_Window( X, Y, W, H, label )
	{
		mLayout = new Fl_GridLayout( 10, 14 );
		mXAxis = new Fl_X_Axis( 0,0,0,0, "Time (sec)" );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mLayout->AddMultiCellWidget( mXAxis, 0, 8, 12, 1  );				
		mLayout->MakeHeightFixed();

		mYAxis = new Fl_Y_Axis( 0,0,0,0, "Amplitude" );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mLayout->AddMultiCellWidget( mYAxis, 12, 0, 1, 8 );
		mLayout->MakeWidthFixed();

		mXSlider = new Fl_ZoomSlider( 0,0,0,0, FL_HORIZONTAL );
		mLayout->AddMultiCellWidget( mXSlider, 0, 9, 12, 1 );
		mLayout->MakeHeightFixed();
				
		mYSlider = new Fl_ZoomSlider( 0,0,0,0, FL_VERTICAL );
		mLayout->AddMultiCellWidget( mYSlider, 13, 0, 1, 8 );
		mLayout->MakeWidthFixed();

		mDisplay = new Fl_Gl_Single_Display( 0,0,0,0 );
		mDisplay->SetRenderer( mDrawMgr );

		mDisplay->EnableDoubleBuffering();

		mLayout->AddMultiCellWidget( mDisplay, 0,0, 12, 8 );

		// Signal and Slot connections

		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
				
		mLayout->end();

		add_resizable( *mLayout );

		end();
				
	}

	void Fl_SinTracks::OnNewTrackList( SineTrackList& list, TSize framelen )
	{
		mDrawMgr.CacheData( list );
		mDisplay->SetLeft( 0 );
		mDisplay->SetRight( framelen );
	}

	void Fl_SinTracks::OnNewRange( TData specRange )
	{
		mYAxis->minimum( 0 );
		mYAxis->maximum( specRange );
		mDisplay->SetBottom( 0 );
		mDisplay->SetTop( specRange );
	}

	void Fl_SinTracks::OnNewDuration( TTime begin, TTime end )
	{
		mXAxis->minimum( begin );
		mXAxis->maximum( end );
	}

	void Fl_SinTracks::resize( int x, int y, int w, int h )
	{
		Fl_Widget::resize( x, y, w, h );

		mLayout->resize( x, y, w, h );

		redraw();
	}

	Fl_SinTracks::~Fl_SinTracks( )
	{
	}

	void Fl_SinTracks::Show()
	{
		show();
	}

	void Fl_SinTracks::Hide()
	{
		hide();
	}
}
