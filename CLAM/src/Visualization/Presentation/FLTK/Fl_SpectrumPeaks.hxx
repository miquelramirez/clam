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
