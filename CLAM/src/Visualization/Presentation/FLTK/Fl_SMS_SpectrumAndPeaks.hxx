/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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
