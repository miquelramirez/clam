#include "Fl_SMS_FundFreq_Browser.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_SMS_Gl_Single_Browsable_Display.hxx"

namespace CLAMVM
{
	Fl_SMS_FundFreq_Browser::Fl_SMS_FundFreq_Browser( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H ), mDisplay( NULL ), mTooltipFmtStr( "time %.3f secs freq. %.2f Hz" )
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

		mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_SMS_FundFreq_Browser::OnRefreshTooltip );

		// Signal and Slot connections

		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
				
		end();
		
		HandleDisplaySelection.Wrap( this, &Fl_SMS_FundFreq_Browser::OnDisplaySelectedXValue );
		SetSelectedXValue.Wrap( this, &Fl_SMS_FundFreq_Browser::OnSetSelectedXValue );

		NewTrajectory.Wrap( this, &Fl_SMS_FundFreq_Browser::OnNewTrajectory );
		NewTimeSpan.Wrap( this, &Fl_SMS_FundFreq_Browser::OnNewTimeSpan );
		NewFreqRangeHint.Wrap( this, &Fl_SMS_FundFreq_Browser::OnNewFreqRangeHint );

		mWorldSpaceCoords.mTop = 1;
		mWorldSpaceCoords.mBottom = 0;
		
		mYAxis->maximum( 1 );
		mYAxis->minimum( 0 );


	}

	Fl_SMS_FundFreq_Browser::~Fl_SMS_FundFreq_Browser()
	{
	}

	void Fl_SMS_FundFreq_Browser::OnDisplaySelectedXValue( double value )
	{
		// towards the outer world
		double sampleTime = ( value/(double)mFrames) * (mEndTime - mBeginTime) + mBeginTime;
		SelectedXValue.Emit( sampleTime );
	}

	void Fl_SMS_FundFreq_Browser::OnSetSelectedXValue( double centerTime )
	{
		// towards the display
		double frameIndex = ( centerTime / ( mEndTime - mBeginTime ) ) * (mFrames-1);
		ChangeSelectedXValue.Emit( frameIndex + 0.5 );

	}

	void Fl_SMS_FundFreq_Browser::OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen )
	{
		double wX = (((double)sx / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
		double wY = mYAxis->maximum() - (((double)sy / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));

		snprintf( txtBuffer, maxLen,  mTooltipFmtStr.c_str(),  wX, wY );	
	}

	void Fl_SMS_FundFreq_Browser::Show()
	{
		show();
	}

	void Fl_SMS_FundFreq_Browser::Hide()
	{
		hide();
	}

	int Fl_SMS_FundFreq_Browser::handle( int event )
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
			mDisplay = new Fl_SMS_Gl_Single_Browsable_Display( x(),y(),w()-50,h()-50 );
			mDisplay->SetRenderer( mDrawMgr );
			mDisplay->EnableDoubleBuffering();
			mDisplay->SetPainting();
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

			mDisplay->SelectedXValue.Connect( HandleDisplaySelection );
			ChangeSelectedXValue.Connect( mDisplay->SetSelectedXValue );

			mXAxis->resize( x(), y()+h()-50,w()-50, 30);
			mYAxis->resize( x()+w()-50, y(), 30, h() - 50);
			mXSlider->resize( x(), y() +h() -20, w() -50, 20);
			mYSlider->resize( x()+w()-20, y(), 20, h() -50 );


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


	void Fl_SMS_FundFreq_Browser::OnNewTrajectory( const CLAM::Array< TimeFreqMagPoint >& trajectory, CLAM::TData sampleRate )
	{
		mDrawMgr.CacheData( trajectory );

		mWorldSpaceCoords.mRight = trajectory.Size();
		mWorldSpaceCoords.mLeft = 0;

		mFrames = trajectory.Size();

		if ( mDisplay )
			mDisplay->redraw();

		redraw();
	}

	void Fl_SMS_FundFreq_Browser::OnNewFreqRangeHint( CLAM::TData lower, CLAM::TData higher )
	{
		mYAxis->minimum( lower );
		mYAxis->maximum( higher );
		mWorldSpaceCoords.mTop = higher;
		mWorldSpaceCoords.mBottom = lower;

		if ( mDisplay )
		{
			mDisplay->SetTop( higher );
			mDisplay->SetBottom( lower );
			mDisplay->redraw();
		}
	}
	

	void Fl_SMS_FundFreq_Browser::OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end )
	{
		mXAxis->minimum( begin );
		mXAxis->maximum( end );
		mBeginTime = begin;
		mEndTime = end;

		if ( mDisplay )
			mDisplay->redraw();

		redraw();
	}
}
