#include "Fl_SMS_SpectrumAndPeaks.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_Gl_Multi_Display.hxx"
#include <algorithm>
#include <FL/Fl_Light_Button.H>

namespace CLAMVM
{

	Fl_SMS_SpectrumAndPeaks::Fl_SMS_SpectrumAndPeaks( int X, int Y, int W, int H, const char* label )
		: Fl_Group( X, Y, W, H, label ), mDisplay( NULL ), mImposterBox( NULL ),
			mTooltipFmtStr( "freq %6.f Hz amp. %4.2f dB")
	{

		mXAxis = new Fl_X_Axis( X,Y+H-50,W-50, 30  );
		mXAxis->align( FL_ALIGN_BOTTOM );
		mXAxis->scale( FL_AXIS_LIN );
		mXAxis->minimum( 0.0f );
		mXAxis->maximum( 1.0f );
		mXAxis->label_format( "%g" );
		mXAxis->label_step( 10 );
		mXAxis->label_size( 9 );
		mXAxis->axis_color( FL_BLACK );
		mXAxis->axis_align( FL_AXIS_BOTTOM|FL_AXIS_LINE);

		mYAxis = new Fl_Y_Axis( X+W-50,Y,30,H-50 );
		mYAxis->align( FL_ALIGN_LEFT );
		mYAxis->scale( FL_AXIS_LIN );
		mYAxis->minimum( -1.0 );
		mYAxis->maximum( 1.0 );
		mYAxis->label_format( "%g" );
		mYAxis->label_step( 10 );
		mYAxis->label_size( 9 );
		mYAxis->axis_color( FL_BLACK );
		mYAxis->axis_align( FL_AXIS_RIGHT | FL_AXIS_LINE );

		mXSlider = new Fl_ZoomSlider( X,Y+H-20,W-50,20, FL_HORIZONTAL );
		mYSlider = new Fl_ZoomSlider( X+W-20,Y,20,H-50, FL_VERTICAL );

		mTooltipTracker.RenderTooltipText.Wrap( this, &Fl_SMS_SpectrumAndPeaks::OnRefreshTooltip );

		mImposterBox = new Fl_Box( X, Y, W-50, H-50 );
		resizable( mImposterBox );

		// Signal and Slot connections

		mXSlider->SpanChanged.Connect( mXAxis->AdjustRange );
		mYSlider->SpanChanged.Connect( mYAxis->AdjustRange );
		
		mShowPeaksBtn = new Fl_Light_Button( X+W-40, Y+H-20, 40, 20, "Peaks" );
		mShowPeaksBtn->labelsize( 9 );
		mShowPeaksBtn->set();
		mShowPeaksBtn->selection_color(FL_RED);
		mShowPeaksBtn->when( FL_WHEN_CHANGED );
		mShowPeaksBtn->callback( &sShowPeaksBtn_cb, this );

		end();
				
		mSpectrumDrawMgr.SetDetailThreshold( 500 );				
		mWorldSpaceCoords.mLeft = -1.0;
		mWorldSpaceCoords.mRight = 1.0;
		mWorldSpaceCoords.mTop = 1.0;
		mWorldSpaceCoords.mBottom = -1.0;

	}

	void Fl_SMS_SpectrumAndPeaks::OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen )
	{
		double wX = (((double)sx / (double)mDisplay->w())*(fabs(mXAxis->maximum()-mXAxis->minimum()))) + mXAxis->minimum();
		double wY = mYAxis->maximum() - (((double)sy / (double)mDisplay->h())*(fabs(mYAxis->maximum()-mYAxis->minimum())));

		snprintf( txtBuffer, maxLen,  mTooltipFmtStr.c_str(),  wX, wY );	

	}

	void Fl_SMS_SpectrumAndPeaks::sShowPeaksBtn_cb( Fl_Widget* w, void* data )
	{
		Fl_SMS_SpectrumAndPeaks* pContainer = ( Fl_SMS_SpectrumAndPeaks* )data;
		if ( pContainer->mPeaksDrawMgr.IsEnabled() )
			pContainer->mPeaksDrawMgr.Disable();
		else
			pContainer->mPeaksDrawMgr.Enable();
		pContainer->mDisplay->redraw();
	}

	int Fl_SMS_SpectrumAndPeaks::handle( int evtCode )
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
			
			mDisplay = new Fl_Gl_Multi_Display( x(), y(), w()-50, h()-50 );
		
			mDisplay->AddRenderer( mSpectrumDrawMgr );
			mDisplay->AddRenderer(mPeaksDrawMgr);
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
			mShowPeaksBtn->resize( x() + w() -40, y() + h() -20, 40, 20);
	
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

	Fl_SMS_SpectrumAndPeaks::~Fl_SMS_SpectrumAndPeaks( )
	{
	}

	void Fl_SMS_SpectrumAndPeaks::OnNewSpectrum( const DataArray& array, TData spectralRange )
	{
		mSpectrumDrawMgr.CacheData( array );
		mWorldSpaceCoords.mRight = array.Size() - 2;
		mWorldSpaceCoords.mLeft = 0;
		mWorldSpaceCoords.mTop = 0;
		mWorldSpaceCoords.mBottom = -150;
		mXAxis->minimum( 0 );
		mXAxis->maximum( spectralRange );

		// We check here for -1.0INFs, clamping the magnitude to -200 dB
		mYAxis->minimum( -150 );
				
		mYAxis->maximum( 0 );
		mPeaksDrawMgr.SetBinNumber( array.Size() );
		mPeaksDrawMgr.SetSpectralRange ( spectralRange );
		if ( mDisplay )
			mDisplay->redraw();
		redraw();
	}

	void Fl_SMS_SpectrumAndPeaks::SetSpectralRange( double spectralRange )
	{
		// MRJ: This is not the way it is meant to... but we want it drawn
		// right?
		mXAxis->minimum( 0 );
		mXAxis->maximum( spectralRange );
		// We check here for -1.0INFs, clamping the magnitude to -200 dB
		mYAxis->minimum( -150 );
		mYAxis->maximum( 0 );
		mWorldSpaceCoords.mRight = 510;
		mWorldSpaceCoords.mLeft = 0;
		mWorldSpaceCoords.mTop = 0;
		mWorldSpaceCoords.mBottom = -150;

		mPeaksDrawMgr.SetSpectralRange( spectralRange );
	}

	void Fl_SMS_SpectrumAndPeaks::OnNewPeakArray( const Array<Partial>& partArray )
	{
		mPeaksDrawMgr.CacheData( partArray );
		redraw();
	}

	void Fl_SMS_SpectrumAndPeaks::Show()
	{
		show();
	}

	void Fl_SMS_SpectrumAndPeaks::Hide()
	{
		hide();
	}
}
