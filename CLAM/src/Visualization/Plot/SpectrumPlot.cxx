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

#include "SpectrumPlot.hxx"
#include "Spectrum.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "Fl_SMS_Spectrum.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	SpectrumPlot::SpectrumPlot( )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new LogMagSpectrumAdapter;
		mpWidget = new Fl_SMS_Spectrum( 0, 0, 100, 100 );

		mpAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );

		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumPlot::SpectrumPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new LogMagSpectrumAdapter;
		mpWidget = new Fl_SMS_Spectrum( 0, 0, 100, 100 );

		mpAdapter->ObjectPublished.Connect( mpWidget->SetSpectrum );

		SetName( name  );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	SpectrumPlot::~SpectrumPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		delete mpAdapter;
	}

	void SpectrumPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void SpectrumPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void SpectrumPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void SpectrumPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetYRange( ymin, ymax );
	}

	void SpectrumPlot::SetTooltipFormat( const char* tooltipFmt )
	{
		mpWidget->SetTooltipFormat( tooltipFmt );
	}

	void SpectrumPlot::SetData( const CLAM::Spectrum& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
}
