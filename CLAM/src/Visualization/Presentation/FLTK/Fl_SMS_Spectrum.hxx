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

#ifndef __FL_SMS_SPECTRUM__
#define __FL_SMS_SPECTRUM__

#include <string>
#include <FL/Fl_Group.H>
#include "SpectrumPresentation.hxx"
#include "SpectrumRenderingManager.hxx"
#include "TooltipTracker2D.hxx"
#include <FL/Fl_Box.H>

namespace CLAMVM
{
	// forwards declarations
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_Gl_Single_Display;

	class Fl_SMS_Spectrum : public SpectrumPresentation, public Fl_Group
	{
	private:
		Fl_X_Axis*                  mXAxis;
		Fl_Y_Axis*                  mYAxis;
		Fl_ZoomSlider*              mXSlider;
		Fl_ZoomSlider*              mYSlider;
		Fl_Gl_Single_Display*       mDisplay;
		SpectrumRenderingManager    mDrawMgr;
		TooltipTracker2D            mTooltipTracker;
		Fl_Box*                     mImposterBox;
		DataBoundBox                mWorldSpaceCoords;
		std::string                 mTooltipFmtStr;
		double                      mMinMag;
		double                      mMaxMag;

	protected:
		
		virtual void OnNewSpectrum( const DataArray&, TData );
		
		void OnRefreshTooltip( int sx, int sy, char* txtBuff, int maxLen );

	public:
		Fl_SMS_Spectrum( int X, int Y, int W, int H, const char* label = 0 );
		~Fl_SMS_Spectrum();
		
		int handle( int eventCode );

		void Show();
		void Hide();

		void SetTooltipFormat( const char* fmtStr );
		void SetYRange( double minMag, double maxMag );
	};

	// inlines

	inline void Fl_SMS_Spectrum::SetTooltipFormat( const char* fmtStr )
	{
		mTooltipFmtStr = fmtStr;
	}

	inline void Fl_SMS_Spectrum::SetYRange( double minMag, double maxMag )
	{
		mMinMag = minMag;
		mMaxMag = maxMag;
	}
}

#endif // Fl_SMS_Spectrum.hxx
