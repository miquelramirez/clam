#include "Fl_Plot.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_Gl_2DSurface.hxx"
#include "GLRenderingManager.hxx"

namespace CLAMVM
{
	Fl_Plot::Fl_Plot()
		: Fl_Group( 0, 0, 100, 100 ), mDisplay( NULL ),
		  mTipFormatString( "x= %4.2f,f(x)= %6.2f" )
	{
		CreateContent();
		resize( 0, 0, 320, 200 );
		mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_Plot::OnRefreshTooltip );

		SetXRange.Wrap( this, &Fl_Plot::OnNewXRange );
		SetYRange.Wrap( this, &Fl_Plot::OnNewYRange );

		// Signal and Slot connections
		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
				
		end();
		//mDrawMgr.SetDetailThreshold( 500 );
		mWSC.mLeft = -1.0;
		mWSC.mRight = 1.0;
		mWSC.mTop = 1.0;
		mWSC.mBottom = -1.0;

	}

	Fl_Plot::~Fl_Plot()
	{
	}

	int Fl_Plot::handle( int evtCode )
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
			
			mDisplay = CreateDisplay( x(), y(), w()-50, h()-50 );
			AddRenderersToDisplay( mDisplay );
			mDisplay->EnableDoubleBuffering();
			mDisplay->SetWorldSpace( mWSC.mRight,
						 mWSC.mLeft,
						 mWSC.mTop, 
						 mWSC.mBottom );
			mDisplay->end();
			add( mDisplay );
			resizable( mDisplay );
			mTooltipTracker.Track( mDisplay );
			mTooltipTracker.ForceText( "idle" );
			mXSlider->SpanChanged.Connect( mDisplay->AdjustXAxis );				
			mYSlider->SpanChanged.Connect( mDisplay->AdjustYAxis );
			PositionWidgets( x(), y(), w(), h() );
			
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

	void Fl_Plot::SetTipFormatString( const char* fmtString )
	{
		mTipFormatString = fmtString;
	}

	void Fl_Plot::CreateContent()
	{
		mXAxis = new Fl_X_Axis( 0, 0, 20, 30 );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM | FL_AXIS_LINE );

		mYAxis = new Fl_Y_Axis( 0, 0, 20, 30 );
		mYAxis->align( FL_ALIGN_RIGHT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( 0, 0, 50, 20, FL_HORIZONTAL );
		mYSlider = new Fl_ZoomSlider( 0, 0, 20, 20, FL_VERTICAL );

		mImposterBox = new Fl_Box( 0, 0, 10, 10 );
		resizable( mImposterBox );

	}

	void Fl_Plot::PositionWidgets( int X, int Y, int W, int H )
	{
		mXAxis->resize( X+5, Y + H - 50 , W - 55, 30 );
		mYAxis->resize( X+W-50, Y+5, 30, H-55 );
		mXSlider->resize( X, Y+H-20, W-50, 20 );
		mYSlider->resize( X+W-20, Y, 20, H-50 );
		
		if ( mDisplay )
		{
			mDisplay->resize( X+5, Y+5, W-55, H-55 );
		}
		else
			mImposterBox->resize( X+5, Y+5, W-50-5, H-50-5 );
	}

	void Fl_Plot::resize( int X, int Y, int W, int H )
	{
		
		Fl_Group::resize( 0, 0, W, H );

		PositionWidgets( X, Y, W, H );

	}

	void Fl_Plot::Show()
	{
		show();
	}

	void Fl_Plot::Hide()
	{
		hide();
	}

	void Fl_Plot::OnNewXRange( double xmin, double xmax )
	{
		mWSC.mLeft = xmin;
		mWSC.mRight = xmax;
		mXAxis->minimum( xmin );
		mXAxis->maximum( xmax );

		redraw();
	}

	void Fl_Plot::OnNewYRange( double ymin, double ymax )
	{
		mWSC.mTop = ymax;
		mWSC.mBottom = ymin;
		mYAxis->minimum( ymin );
		mYAxis->maximum( ymax );

		redraw();
	}

	void Fl_Plot::OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen  )
	{
		double wX = (((double)sx / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
		double wY = mYAxis->maximum() - (((double)sy / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));
		
		snprintf( txtBuffer, maxLen,  mTipFormatString.c_str(),  wX, wY );	

	}


}
