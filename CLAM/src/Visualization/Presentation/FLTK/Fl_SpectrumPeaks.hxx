#ifndef __FLSPECTRUMPEAKS__
#define __FLSPECTRUMPEAKS__

#include <FL/Fl_Window.H>
#include "SpectralPeaksPresentation.hxx"
#include "SpectrumRenderingManager.hxx"
#include "PeaksRenderingManager.hxx"

namespace CLAMVM
{
		class Fl_X_Axis;
		class Fl_Y_Axis;
		class Fl_ZoomSlider;
		class Fl_GridLayout;
		class Fl_Gl_Multi_Display;

		class Fl_SpectrumPeaks 
			: public SpectrumPlusPeaksPresentation, public Fl_Window
		{
		private:
				Fl_GridLayout*               mLayout;
				Fl_X_Axis*                   mXAxis;
				Fl_Y_Axis*                   mYAxis;
				Fl_ZoomSlider*               mXSlider;
				Fl_ZoomSlider*               mYSlider;
				Fl_Gl_Multi_Display*         mDisplay;
				SpectrumRenderingManager     mSpectrumDrawMgr;
				PeaksRenderingManager        mPeaksDrawMgr;

		protected:

				virtual void OnNewSpectrum( const DataArray&, TData spectralRange );
				virtual void OnNewPeakArray( const Array<Partial>& );

		public:

				Fl_SpectrumPeaks( int X, int Y, int W, int H, const char* label = 0 );
				~Fl_SpectrumPeaks();

				virtual void resize( int X, int Y, int W, int H );
				virtual void Show();
				virtual void Hide();

		};
}

#endif // Fl_SpectrumPeaks.hxx
