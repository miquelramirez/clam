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

#include "PlotSpectralPeaks.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakArrayAdapter.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_SpectrumPeaks.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{
	void plot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks,
		   const char* label )
	{
		LogMagSpectrumAdapter specAdapter;
		SpectralPeakArrayAdapter specPeaksAdapter;
		
		specAdapter.BindTo( s );
		specPeaksAdapter.BindTo( speaks );

		Fl_SpectrumPeaks presWidget( 100, 100, 640, 480, label);
		
		presWidget.AttachTo( specAdapter, specPeaksAdapter );
		
		specAdapter.Publish();
		specPeaksAdapter.Publish();

		presWidget.Show();
		
		// Run widget toolkit wrapper:
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();		
	}

	void deferredPlot( const CLAM::Spectrum& s, const CLAM::SpectralPeakArray& speaks, const char* label  )
	{
		LogMagSpectrumAdapter* specAdapter = new LogMagSpectrumAdapter;
		SpectralPeakArrayAdapter* specPeaksAdapter = new SpectralPeakArrayAdapter;
		
		specAdapter->BindTo( s );
		specPeaksAdapter->BindTo( speaks );

		Fl_SpectrumPeaks* presWidget = new Fl_SpectrumPeaks( 100, 100, 640, 480, label);
		
		presWidget->AttachTo( *specAdapter, *specPeaksAdapter );
		
		specAdapter->Publish();
		specPeaksAdapter->Publish();
	
		Plotter::Hold( specAdapter, presWidget );

		delete specPeaksAdapter;
	}
}
