#include "Fl_SpectrumPeaks.hxx"
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include "Fl_Gl_Multi_Display.hxx"
#include <algorithm>

namespace CLAMVM
{

		Fl_SpectrumPeaks::Fl_SpectrumPeaks( int X, int Y, int W, int H, const char* label )
				: Fl_Window( X, Y, W, H, label )
		{
				mLayout = new Fl_GridLayout( 10, 14 );
				mXAxis = new Fl_X_Axis( 0,0,0,0 );
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

				mYAxis = new Fl_Y_Axis( 0,0,0,0 );
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

				mDisplay = new Fl_Gl_Multi_Display( 0,0,0,0 );
				mDisplay->AddRenderer( mSpectrumDrawMgr );
				mDisplay->AddRenderer( mPeaksDrawMgr );
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
				
				mSpectrumDrawMgr.SetDetailThreshold( 50 );				

		}

		void Fl_SpectrumPeaks::resize( int x, int y, int w, int h )
		{
			Fl_Widget::resize( x, y, w, h );

			mLayout->resize( x, y, w, h );

			redraw();
		}

		Fl_SpectrumPeaks::~Fl_SpectrumPeaks( )
		{
		}

		void Fl_SpectrumPeaks::OnNewSpectrum( const DataArray& array, TData spectralRange )
		{
				mSpectrumDrawMgr.CacheData( array );
				TData maxMag = 0; // Fixed at 0 dBs
				TData minMag = *std::min_element( array.GetPtr(), array.GetPtr()+array.Size() );
				const TData offsetPercentil = 0.2f; // 20%
				TData maxOffset = fabs( maxMag - minMag ) * offsetPercentil;
				mDisplay->SetWorldSpace( array.Size() - 2, 0, maxMag + maxOffset, minMag );
				mXAxis->minimum( 0 );
				mXAxis->maximum( spectralRange );

				// We check here for -1.0INFs, clamping the magnitude to -200 dB
				mYAxis->minimum( ( minMag < -200 ) ? -200 : minMag );
				
				mYAxis->maximum( 0 );
				mPeaksDrawMgr.SetBinNumber( array.Size() );
				mPeaksDrawMgr.SetSpectralRange ( spectralRange );
				redraw();
		}

		void Fl_SpectrumPeaks::OnNewPeakArray( const Array<Partial>& partArray )
		{
				mPeaksDrawMgr.CacheData( partArray );
				redraw();
		}

		void Fl_SpectrumPeaks::Show()
		{
				show();
		}

		void Fl_SpectrumPeaks::Hide()
		{
				hide();
		}
}
