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

#include "PlotSinTracks.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Array.hxx"
#include "SinTracksAdapter.hxx"
#include "Fl_SinTracks.hxx"
#include "WidgetTKWrapper.hxx"
#include "Plotter.hxx"

namespace CLAMVM
{

	void plot( const CLAM::Segment& model, const char* label )
	{
		CLAMVM::SinTracksAdapter ma;

		ma.BindTo( model );

		// create the widget
		CLAMVM::Fl_SinTracks presWidget( 100, 100, 640, 480, label );
		
		presWidget.AttachTo( ma );

		ma.Publish();

		presWidget.Show();

		// Run widget toolkit wrapper:
		CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();
		
	}

	void plot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label )
	{
		CLAMVM::SinTracksAdapter ma;

		ma.BindTo( model, sampleRate );

		// create the widget
		CLAMVM::Fl_SinTracks presWidget( 100, 100, 640, 480, label );
		
		presWidget.AttachTo( ma );

		ma.Publish();

		presWidget.Show();

		// Run widget toolkit wrapper:
		CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();

	}

	void deferredPlot( const CLAM::Segment& model, const char* label )
	{
		CLAMVM::SinTracksAdapter* ma = new SinTracksAdapter;

		ma->BindTo( model );

		// create the widget
		CLAMVM::Fl_SinTracks*  presWidget = new CLAMVM::Fl_SinTracks( 100, 100, 640, 480, label );
		
		presWidget->AttachTo( *ma );

		ma->Publish();

		Plotter::Hold( ma, presWidget );
	}

	void deferredPlot( const CLAM::Array< CLAM::SpectralPeakArray >& model, double sampleRate, const char* label )
	{
		CLAMVM::SinTracksAdapter* ma = new SinTracksAdapter;

		ma->BindTo( model, sampleRate );

		// create the widget
		CLAMVM::Fl_SinTracks*  presWidget = new CLAMVM::Fl_SinTracks( 100, 100, 640, 480, label );
		
		presWidget->AttachTo( *ma );

		ma->Publish();

		Plotter::Hold( ma, presWidget );		
	}

}
