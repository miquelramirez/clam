#ifndef __FL_SMS_SPECTRUMANDPEAKS__
#define __FL_SMS_SPECTRUMANDPEAKS__

#include <string>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "SpectralPeaksPresentation.hxx"
#include "SpectrumRenderingManager.hxx"
#include "PeaksRenderingManager.hxx"
#include "TooltipTracker2D.hxx"


class Fl_Light_Button;

namespace CLAMVM
{
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_GridLayout;
	class Fl_Gl_Multi_Display;

	class Fl_SMS_SpectrumAndPeaks 
		: public SpectrumPlusPeaksPresentation, public Fl_Group
	{
	private:
		Fl_X_Axis*                   mXAxis;
		Fl_Y_Axis*                   mYAxis;
		Fl_ZoomSlider*               mXSlider;
		Fl_ZoomSlider*               mYSlider;
		Fl_Gl_Multi_Display*         mDisplay;
		Fl_Light_Button*             mShowPeaksBtn;
		Fl_Box*                      mImposterBox;
		SpectrumRenderingManager     mSpectrumDrawMgr;
		PeaksRenderingManager        mPeaksDrawMgr;
		TooltipTracker2D             mTooltipTracker;
		DataBoundBox                 mWorldSpaceCoords;
		std::string                  mTooltipFmtStr;

	protected:

		virtual void OnNewSpectrum( const DataArray&, TData spectralRange );
		virtual void OnNewPeakArray( const Array<Partial>& );

		void OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen );

		static void sShowPeaksBtn_cb( Fl_Widget* w, void* data );

	public:

		Fl_SMS_SpectrumAndPeaks( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SMS_SpectrumAndPeaks();

		int handle( int evtCode );

		virtual void Show();
		virtual void Hide();

		void SetTooltipFormat( const char* fmtStr );
		void SetSpectralRange( double spectralRange );
		void SetYRange( double minMag, double maxMag );
	};

	// inlines 
	inline void Fl_SMS_SpectrumAndPeaks::SetTooltipFormat( const char* fmtStr )
	{
		mTooltipFmtStr = fmtStr;
	}
}

#endif // Fl_SpectrumPeaks.hxx
