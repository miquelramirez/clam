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

#include "Plot.hxx"
#include "Assert.hxx"
#include "Fl_SinglePlot.hxx"
#include "SequenceRenderingManager.hxx"

namespace CLAMVM
{

	Plot::Plot()
		: mRenderer( NULL )
	{
		CreateWidget();
		mRenderer = new SequenceRenderingManager;		
		Fl_SinglePlot* pWidget = static_cast< Fl_SinglePlot* >( GetWidget() );
		pWidget->SetRenderingManager( mRenderer );		
		RegisterAnonymously();
	}

	Plot::Plot( std::string name )
		: mRenderer( NULL )
	{
		CreateWidget();
		mRenderer = new SequenceRenderingManager;
		Fl_SinglePlot* pWidget = static_cast< Fl_SinglePlot* >( GetWidget() );
		pWidget->SetRenderingManager( mRenderer );		
		SetName( name );
		RegisterAs( name );
	}



	Plot::~Plot()
	{
		if ( mRenderer )
			delete mRenderer;
	}

	void Plot::CreateWidget( )
	{
		SetWidget( new Fl_SinglePlot() );
		GetWidget()->label( GetName().c_str() );
	}

	void Plot::SetColor( Color c )
	{
		mRenderer->SetLineColor( c );
	}


	void Plot::SetYRange( double ymin, double ymax )
	{
		GenericPlot::SetYRange( ymin, ymax );
		mRenderer->SetDataYRange( ymin, ymax );				
	}
	

	void Plot::SetData( const CLAM::Array<CLAM::TData>& data, double xmin, double xmax )
	{
		mRenderer->SetDataXRange( xmin, xmax );
		GetWidget()->SetXRange( xmin, xmax );
		mRenderer->CacheData( data );	       
	}
       
	void Plot::SetData( const CLAM::BPF& data, int samples )
	{
		CLAM::DataArray sampledBPF;

		CLAM::TData start = data.GetXValue( 0 );
		CLAM::TData end = data.GetXValue( data.Size() - 1 );

		CLAM::TData dx = fabs( end - start ) / TData( samples );

		sampledBPF.Resize( samples+1 );
		sampledBPF.SetSize( samples+1 );

		CLAM::TData x = start;

		for ( int i = 0; i < sampledBPF.Size(); i++ )
		{
			sampledBPF[i] = data.GetValue( x );
			x += dx;
		}

		mRenderer->CacheData( sampledBPF );
		GetWidget()->SetXRange( start, end );
		mRenderer->SetDataXRange( start, end );
	}
	
	
}
