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

#include "AudioPlot.hxx"
#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "SystemPlots.hxx"

namespace CLAMVM
{
	AudioPlot::AudioPlot()
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new AudioAdapter;
		mpWidget = new Fl_SMS_Browsable_Playable_Audio( 0, 0, 100, 100 );
		mpAdapter->ObjectPublished.Connect( mpWidget->SetAudio );
		
		SetName( SystemPlots::GenerateNewUniqueName() );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	AudioPlot::AudioPlot( std::string name )
		: mpAdapter( NULL ), mpWidget( NULL )
	{
		mpAdapter = new AudioAdapter;
		mpWidget = new Fl_SMS_Browsable_Playable_Audio( 0, 0, 100, 100 );
		mpAdapter->ObjectPublished.Connect( mpWidget->SetAudio );		

		SetName( name );
		SystemPlots::RegisterWidget( GetName(), mpWidget );
	}

	AudioPlot::~AudioPlot()
	{
		if ( mpWidget && mpWidget->parent() == NULL )
			delete mpWidget;
		delete mpAdapter;
	}

	void AudioPlot::SetPosition( int x, int y )
	{
		mpWidget->position( x, y );
	}

	void AudioPlot::SetSize( int w, int h )
	{
		mpWidget->size( w, h );
	}

	void AudioPlot::SetLabel( const char* label )
	{
		mpWidget->label( label );
	}

	void AudioPlot::SetYRange( double ymin, double ymax )
	{
		mpWidget->SetYRange( ymin, ymax );
	}

	void AudioPlot::SetTooltipFormat( const char* tooltipFmt )
	{
		mpWidget->SetTooltipFormat( tooltipFmt );
	}

	void AudioPlot::SetData( const CLAM::Audio& obj )
	{
		mpAdapter->BindTo( obj );
		mpAdapter->Publish();
	}
}
