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

#include "FundFreqPlot.hxx"
#include "Segment.hxx"
#include "FundamentalAdapter.hxx"
#include "Fl_SMS_FundFreq_Browser.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	FundFreqPlot::FundFreqPlot()
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new FundamentalAdapter;
		mpWidget = new Fl_SMS_FundFreq_Browser( 0, 0, 100, 100 );
		
		mpAdapter->TrajectoryExtracted.Connect( mpWidget->NewTrajectory );
		mpAdapter->TimeSpanChanged.Connect( mpWidget->NewTimeSpan );

		mpWidget->NewFreqRangeHint( 50, 8000 );

		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );

	}

	FundFreqPlot::FundFreqPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new FundamentalAdapter;
		mpWidget = new Fl_SMS_FundFreq_Browser( 0, 0, 100, 100 );
		
		mpAdapter->TrajectoryExtracted.Connect( mpWidget->NewTrajectory );
		mpAdapter->TimeSpanChanged.Connect( mpWidget->NewTimeSpan );

		mpWidget->NewFreqRangeHint( 50, 8000 );

		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );

	}

	FundFreqPlot::~FundFreqPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		if ( mpAdapter )
			delete mpAdapter;		
	}

	void FundFreqPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void FundFreqPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void FundFreqPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void FundFreqPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->NewFreqRangeHint( ymin, ymax );
	}

	void FundFreqPlot::SetTooltipFormat( const char* fmtStr )
	{
		mpWidget->SetTooltipFormat( fmtStr );
	}

	void FundFreqPlot::SetData( const CLAM::Segment& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
	
}
