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
 * This program is distributed in thFl_Smart_Tile.e hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Fl_SMS_Spectrum.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include "Fl_Gl_Single_Display.hxx"
#include <algorithm>

namespace CLAMVM
{
	Fl_SMS_Spectrum::Fl_SMS_Spectrum( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H, label ), mDisplay( NULL ), 
		  mTooltipFmtStr( "f=%6.f Hz, amp(f)=%4.2f dB" ),
		  mMinMag( -150 ), mMaxMag( 0 )
	{
		mXAxis = new Fl_X_Axis( X, Y + H-50, W-50, 30 );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM | FL_AXIS_LINE );

		mYAxis = new Fl_Y_Axis( X+W-50, Y, 30, H-50 );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( mMinMag );
		mYAxis->maximum( mMaxMag );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( X, Y+H-20, W-50, 20, FL_HORIZONTAL );
		mYSlider = new Fl_ZoomSlider( X+W-20, Y, 20, H-50, FL_VERTICAL );

		mImposterBox = new Fl_Box( X, Y, W-50, H-50 );
		resizable( mImposterBox );

		mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_SMS_Spectrum::OnRefreshTooltip );

		// Signal and Slot connections
		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
				
		end();
		mDrawMgr.SetDetailThreshold( 500 );
		mWorldSpaceCoords.mLeft = -1.0;
		mWorldSpaceCoords.mRight = 1.0;
		mWorldSpaceCoords.mTop = mMaxMag;
		mWorldSpaceCoords.mBottom = mMinMag;
	
	}

	Fl_SMS_Spectrum::~Fl_SMS_Spectrum( )
	{
	}

	void Fl_SMS_Spectrum::OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen  )
	{
		double wX = (((double)sx / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
		double wY = mYAxis->maximum() - (((double)sy / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));

		snprintf( txtBuffer, maxLen,  mTooltipFmtStr.c_str(),  wX, wY );	

	}

	int Fl_SMS_Spectrum::handle( int evtCode )
	{
		if ( evtCode == FL_ENTER  )
		{
			
			if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
				mTooltipTracker.ForceText( "idle" );
			
			return 1;
		}
		else if (  evtCode == FL_MOVE )
		{
			if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
				mTooltipTracker.ForceText( "idle" );
			
			return 1;
			
		}
		else if ( evtCode == FL_SHOW )
		{
			CLAM_ASSERT( mDisplay == NULL, "Precondition violation" );
			mImposterBox->hide();
			
			mDisplay = new Fl_Gl_Single_Display( x(), y(), w()-50, h()-50 );
			mDisplay->SetRenderer( mDrawMgr );
			mDisplay->EnableDoubleBuffering();
			mDisplay->SetWorldSpace( mWorldSpaceCoords.mRight,
						 mWorldSpaceCoords.mLeft,
						 mWorldSpaceCoords.mTop, 
						 mWorldSpaceCoords.mBottom );
			mDisplay->end();
			add( mDisplay );
			resizable( mDisplay );
			mTooltipTracker.Track( mDisplay );
			mTooltipTracker.ForceText( "idle" );
			mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
			mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
			mXAxis->resize( x(), y()+h()-50,w()-50, 30);
			mYAxis->resize( x()+w()-50, y(), 30, h() - 50);
			mXSlider->resize( x(), y() +h() -20, w() -50, 20);
			mYSlider->resize( x()+w()-20, y(), 20, h() -50 );
	
		}
		else if ( evtCode == FL_HIDE )
		{
			if ( mDisplay )
			{
				remove( mDisplay );
				delete mDisplay;
				mDisplay = NULL;
			}

		}
			
		return Fl_Group::handle( evtCode );		
	}

	void Fl_SMS_Spectrum::OnNewSpectrum( const DataArray& array, TData spectralRange )
	{
		mDrawMgr.CacheData( array );
		mWorldSpaceCoords.mRight = array.Size() - 2;
		mWorldSpaceCoords.mLeft = 0;
		mWorldSpaceCoords.mTop = mMaxMag;
		mWorldSpaceCoords.mBottom = mMinMag;
		mXAxis->minimum( 0 );
		mXAxis->maximum( spectralRange );
		mYAxis->minimum( mMinMag );
		mYAxis->maximum( mMaxMag );
		if ( mDisplay )
			mDisplay->redraw();
		redraw();
	}

	void Fl_SMS_Spectrum::Show()
	{
		show();
	}

	void Fl_SMS_Spectrum::Hide()
	{
		hide();
	}
}
