#include "Fl_FundFreq.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_Gl_Single_Display.hxx"

namespace CLAMVM
{
	Fl_FundFreq::Fl_FundFreq( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H, label ), mDisplay( NULL )
	{
		mXAxis = new Fl_X_Axis( X,Y+H-50,W-50, 30 );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mYAxis = new Fl_Y_Axis( X+ W-50,Y,30,H-50 );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( X, Y+H-20,W-50,20, FL_HORIZONTAL );
				
		mYSlider = new Fl_ZoomSlider( X+W-20,Y,20,H-50, FL_VERTICAL );

		mImposterBox = new Fl_Box( X, Y, W-50, H-50 );
		resizable( mImposterBox );

		mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_FundFreq::OnRefreshTooltip );

		// Signal and Slot connections

		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
				
		end();
		
		NewTrajectory.Wrap( this, &Fl_FundFreq::OnNewTrajectory );
		NewTimeSpan.Wrap( this, &Fl_FundFreq::OnNewTimeSpan );

	}

	Fl_FundFreq::~Fl_FundFreq()
	{
	}

	void Fl_FundFreq::OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen )
	{
		double wX = (((double)sx / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
		double wY = mYAxis->maximum() - (((double)sy / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));

		snprintf( txtBuffer, maxLen,  "freq. %.2f Hz time %.3f secs",  wY, wX );	
	}

	void Fl_FundFreq::Show()
	{
		show();
	}

	void Fl_FundFreq::Hide()
	{
		hide();
	}

	int Fl_FundFreq::handle( int event )
	{
		if ( event == FL_ENTER  )
		{
			
			if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
				mTooltipTracker.ForceText( "idle" );
			
			return 1;
		}
		else if ( event == FL_LEAVE )
		{
			if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
				mTooltipTracker.ForceText( "idle" );
			
			return 1;			
		}
		else if (  event == FL_MOVE )
		{
			if ( !mTooltipTracker.HandleMotion( Fl::event_x(), Fl::event_y() ) )
				mTooltipTracker.ForceText( "idle" );
			
			return 1;
			
		}
		else if ( event == FL_SHOW )
		{
			if ( !mDisplay )
			{
				mDisplay = new Fl_Gl_Single_Display( x(),y(),w()-50,h()-50 );
				mDisplay->SetRenderer( mDrawMgr );
				mDisplay->EnableDoubleBuffering();
				mDisplay->SetWorldSpace( mWorldSpaceCoords.mRight,
							 mWorldSpaceCoords.mLeft,
							 mWorldSpaceCoords.mTop, 
							 mWorldSpaceCoords.mBottom );

				mDisplay->end();
				
				add(mDisplay);
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
			
			redraw();

		}
		else if ( event == FL_HIDE )
		{
			if ( mDisplay )
			{
				remove( mDisplay );
				delete mDisplay;
				mDisplay = NULL;
			}
		}
	
		return Fl_Group::handle( event );
	}


	void Fl_FundFreq::OnNewTrajectory( const CLAM::Array< TimeFreqMagPoint >& trajectory, CLAM::TData sampleRate )
	{
		mDrawMgr.CacheData( trajectory );

		mWorldSpaceCoords.mRight = trajectory.Size();
		mWorldSpaceCoords.mLeft = 0;
		mWorldSpaceCoords.mTop = sampleRate/2;
		mWorldSpaceCoords.mBottom = 0;
		
		mYAxis->minimum( 0 );
		mYAxis->maximum( sampleRate/2 );

		if ( mDisplay )
			mDisplay->redraw();
	}
	

	void Fl_FundFreq::OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end )
	{
		mXAxis->minimum( begin );
		mXAxis->maximum( end );
		
		if ( mDisplay )
			mDisplay->redraw();
	}
}
