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

#ifndef __SPECTRUMANDPEAKSPLOT__
#define __SPECTRUMANDPEAKSPLOT__

#include "PlotBase.hxx"

namespace CLAM
{
	class Spectrum;
	class SpectralPeakArray;
}

namespace CLAMVM
{
	class LogMagSpectrumAdapter;
	class SpectralPeakArrayAdapter;
	class Fl_SMS_SpectrumAndPeaks;

	class SpectrumAndPeaksPlot : public PlotBase
	{
	public:
		SpectrumAndPeaksPlot();
		SpectrumAndPeaksPlot( std::string name );
		virtual ~SpectrumAndPeaksPlot( );
		
		void SetPosition( int x, int y );
		void SetSize( int w, int h );
		void SetLabel( const char* label );
		void SetYRange( double ymin, double ymax );
		void SetTooltipFormat( const char* tooltipFmt );
		
		void SetData( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& p );
		void SetData( const CLAM::Spectrum& s );
		void SetData( const CLAM::SpectralPeakArray& p, double spectralRange  );

	protected:
		LogMagSpectrumAdapter*     mpSpectrumAdapter;
		SpectralPeakArrayAdapter*  mpPeakArrayAdapter;
		Fl_SMS_SpectrumAndPeaks*   mpWidget;
	};
}

#endif // SpectrumAndPeaksPlot.hxx
