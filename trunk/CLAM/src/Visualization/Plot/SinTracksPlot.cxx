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

#include "SinTracksPlot.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "SinTracksAdapter.hxx"
#include "Fl_SMS_SinTracks_Browser.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	SinTracksPlot::SinTracksPlot()
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new SinTracksAdapter;
		mpWidget = new Fl_SMS_SinTracks_Browser( 0, 0, 100, 100 );
		mpAdapter->TrackListPublished.Connect( mpWidget->SetTrackList );
		mpAdapter->SpectralRangePublished.Connect( mpWidget->SetSpectralRange );
		mpAdapter->DurationPublished.Connect( mpWidget->SetTimeInterval );

		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SinTracksPlot::SinTracksPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new SinTracksAdapter;
		mpWidget = new Fl_SMS_SinTracks_Browser( 0, 0, 100, 100 );
		mpAdapter->TrackListPublished.Connect( mpWidget->SetTrackList );
		mpAdapter->SpectralRangePublished.Connect( mpWidget->SetSpectralRange );
		mpAdapter->DurationPublished.Connect( mpWidget->SetTimeInterval );

		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SinTracksPlot::~SinTracksPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		if ( mpAdapter )
			delete mpAdapter;
	}

	void SinTracksPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void SinTracksPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void SinTracksPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void SinTracksPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetSpectralRange( fabs( ymax-ymin ) );
	}

	void SinTracksPlot::SetTooltipFormat( const char* fmtStr )
	{
		mpWidget->SetTooltipFormat( fmtStr );
	}

	void SinTracksPlot::SetData( const CLAM::Segment& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}

	void SinTracksPlot::SetData( const CLAM::Array< CLAM::SpectralPeakArray >& obj,
								 double sampleRate )
	{
		mpAdapter->BindTo( obj, sampleRate );
		mpAdapter->Publish();
	}
}
