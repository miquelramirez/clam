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

#include "VisualizationController.hxx"

#include "Segment.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "AudioAdapter.hxx"
#include "LogMagSpectrumAdapter.hxx"

#include "Fl_Browsable_Playable_Audio.hxx"
#include "Fl_SMS_Spectrum.hxx"
#include "Fl_SMS_SinTracks.hxx"

#include "Signalv1.hxx"
#include "Slotv1.hxx"

using namespace CLAMVM;

CLAM::Enum::tEnumValue VisualizationController::sDisplayName[] = {
	{ VisualizationController::eAudioIn, "Input Audio" },
	{ VisualizationController::eAudioOut, "Output Audio" },
	{ VisualizationController::eAudioSinusoidal, "Sinusoidal Part" },
	{ VisualizationController::eAudioResidual, "Residual Part" },
	{ VisualizationController::eSpectrumIn, "Input Spectrum" },
	{ VisualizationController::eSpectrumOut, "Output Spectrum" },
	{ 0, NULL }
};

VisualizationController::VisualizationController(  )
	: mAudioAdapters( eAudioDisplays ), mSpectrumAdapters( eSpectrumDisplays ),
	  mAudioPresentations( eAudioDisplays, 0 ), mSpectrumPresentations( eSpectrumDisplays, 0 ),
	  mDetachCallbackData( eNumDisplays ), mOpenDisplays( eNumDisplays, false ), mCanvas( 0 ),
	  mAnalysisDone( false ), mSinTracksWidget( NULL )
{
	InitDetachCallbackData();
}

VisualizationController::VisualizationController( Fl_Smart_Tile* canvas )
	: mAudioAdapters( eAudioDisplays ), mSpectrumAdapters( eSpectrumDisplays ),
	  mAudioPresentations( eAudioDisplays, 0 ), mSpectrumPresentations( eSpectrumDisplays, 0 ),
	  mDetachCallbackData( eNumDisplays ), mOpenDisplays( eNumDisplays, false ), mCanvas( canvas ),
	  mAnalysisDone( false ), mSinTracksWidget( NULL )
{
	InitDetachCallbackData();
}

void VisualizationController::InitDetachCallbackData()
{
	for( int i = 0; i < eAudioDisplays; i++ )
	{
		// registering callback for notifying the ui the need of 'detaching'
		mDetachCallbackData[ i ].view = ( enum DisplayList ) i;
		mDetachCallbackData[ i ].vc = this;
	}
		
	for( int i = 0; i < eSpectrumDisplays; i++ )
	{
		mDetachCallbackData[ i+eAudioDisplays ].view = ( enum DisplayList )(i + eAudioDisplays);
		mDetachCallbackData[ i+eAudioDisplays ].vc = this;
	}

	mDetachCallbackData[ eSinTracks ].view = eSinTracks;
	mDetachCallbackData[ eSinTracks ].vc = this;
}

VisualizationController::~VisualizationController(  ) 
{ 
}

void VisualizationController::AnalysisDone ( )
{
	mAnalysisDone = true;

	for( int i = 0; i < eAudioDisplays; i++ )
	{
		if( mOpenDisplays[ i ] )
			mAudioPresentations[ i ]->SetPaint();
	}
}

void VisualizationController::AnalysisToDo ( )
{
	mAnalysisDone = false;

	for( int i = 0; i < eAudioDisplays; i++ )
	{
		if( mOpenDisplays[ i ] )
			mAudioPresentations[ i ]->UnsetPaint();
	}
}

void VisualizationController::CloseAll ( )
{
	for( int i = 0; i < eNumDisplays; i++ )
	{
		if( mOpenDisplays[ i ] )
			Detach( ( enum DisplayList )i );
	}
	if( mCanvas )
		mCanvas->redraw();
}

void VisualizationController::_Detach(Fl_Window *w,DetachCallbackData* d)
{
	d->vc->Detach(d->view);
}

void VisualizationController::Detach( enum DisplayList view )
{
	switch( view ) {
	case eAudioIn:
	case eAudioOut:
	case eAudioSinusoidal:
	case eAudioResidual:
		mAudioPresentations[ view ]->hide( );
		if( mCanvas )
		{
			mCanvas->remove( mAudioPresentations[ view ] );
			mCanvas->redraw();
		}
		delete mAudioPresentations[ view ];
		break;

	case eSpectrumIn:
	case eSpectrumOut:
		mSpectrumPresentations[ view-eAudioDisplays ]->hide();
		if( mCanvas )
		{
			mCanvas->remove( mSpectrumPresentations[ view-eAudioDisplays ] );
			mCanvas->redraw();
		}
		delete mSpectrumPresentations[ view-eAudioDisplays ];
		break;
	case eSinTracks:
		mSinTracksWidget->hide();
		if ( mCanvas )
		{
			mCanvas->remove( mSinTracksWidget );
			mCanvas->redraw();
		}
		delete mSinTracksWidget;
		mSinTracksWidget = NULL;
		break;
	}
	mOpenDisplays[ view ] = false;
}

void VisualizationController::Display ( enum DisplayList view )
{
	if( mOpenDisplays[ view ] )
		switch( view ) {
		case eAudioIn:
		case eAudioOut:
		case eAudioSinusoidal:
		case eAudioResidual:
			mAudioPresentations[ view ]->redraw();
			break;

		case eSpectrumIn:
		case eSpectrumOut:
			mSpectrumPresentations[ view ]->redraw();
			break;
		case eSinTracks:
			mSinTracksWidget->redraw();
			break;
		}
}

void VisualizationController::Display( CLAM::Segment& data )
{
	if ( mSinTracksWidget )
	{
		mSinTracksAdapter.BindTo( data );
		mSinTracksAdapter.Publish();
		mSinTracksWidget->redraw();
	}
	else
	{
		int widgetWidth = 800;
		int widgetHeight = 600;
		if( mCanvas )
		{
			widgetWidth = mCanvas->w();
			widgetHeight = mCanvas->h()/(mCanvas->children()+1);
		}

		mSinTracksWidget = new Fl_SMS_SinTracks( 0,0, widgetWidth, widgetHeight);
		mSinTracksWidget->label( "Sinusoidal Tracks" );
		mSinTracksWidget->callback( (Fl_Callback*)_Detach,&mDetachCallbackData[ eSinTracks ] );
		mSinTracksWidget->AttachTo( mSinTracksAdapter );
		mSinTracksAdapter.BindTo( data );

		mSinTracksAdapter.Publish();

		if ( mCanvas )
			mCanvas->add( *mSinTracksWidget );
		mSinTracksWidget->Show();
		if ( mCanvas )
			mCanvas->redraw();
	}
}

void VisualizationController::Display ( enum DisplayList view, Audio& data )
{
	if( mOpenDisplays[ view ] )
	{
		mAudioAdapters[ view ].BindTo( data );
		mAudioAdapters[ view ].Publish();
//		mAudioPresentations[ view ]->redraw();
	}
	else {
		mOpenDisplays[ view ] = true;

		mAudioAdapters[ view ].BindTo( data );

		short width = 800;
		short height = 600;
		if( mCanvas )
		{
			width = mCanvas->w();
			height = mCanvas->h()/(mCanvas->children()+1);
		}
		mAudioPresentations[ view ] = new Fl_Browsable_Playable_Audio( 0, 0, width, height );
		mAudioPresentations[ view ]->label( sDisplayName[ view ].name );
		mAudioPresentations[ view ]->callback( (Fl_Callback*)_Detach, &mDetachCallbackData[ view ] );
		mAudioPresentations[ view ]->AttachTo( mAudioAdapters[ view ] );

		if( mAnalysisDone )
			mAudioPresentations[ view ]->SetPaint();

		//Link Signals with Slots
		mFrameSignal.Connect( mAudioPresentations[ view ]->mRecievingNewFrame );
		mAudioPresentations[ view ]->mSendingMouseEvent.Connect( mSlot );

		mAudioAdapters[ view ].Publish();

		if( mCanvas )
			mCanvas->add( *mAudioPresentations[ view ] );
		mAudioPresentations[ view ]->Show();
		if( mCanvas )
			mCanvas->redraw();
	}
}

void VisualizationController::Display ( enum DisplayList view, Spectrum& data )
{
	if( mOpenDisplays[ view ] )
	{
		mSpectrumAdapters[ view-eAudioDisplays ].BindTo( data );
		mSpectrumAdapters[ view-eAudioDisplays ].Publish();
//		mSpectrumPresentations[ view-eAudioDisplays ]->redraw();
	}
	else {
		mOpenDisplays[ view ] = true;

		mSpectrumAdapters[ view-eAudioDisplays ].BindTo( data );

		short width = 800;
		short height = 600;
		if( mCanvas )
		{
			width = mCanvas->w();
			height = mCanvas->h()/(mCanvas->children()+1);
		}
		mSpectrumPresentations[ view-eAudioDisplays ] = new Fl_SMS_Spectrum( 0, 0, width, height );
		mSpectrumPresentations[ view-eAudioDisplays ]->label( sDisplayName[ view ].name );
		mSpectrumPresentations[ view-eAudioDisplays ]->callback( (Fl_Callback*)_Detach, &mDetachCallbackData[ view ] );
		mSpectrumPresentations[ view-eAudioDisplays ]->AttachTo( mSpectrumAdapters[ view-eAudioDisplays ] );

		mSpectrumAdapters[ view-eAudioDisplays ].Publish();

		if( mCanvas )
			mCanvas->add( *mSpectrumPresentations[ view-eAudioDisplays ] );
		mSpectrumPresentations[ view-eAudioDisplays ]->Show();
		if( mCanvas )
			mCanvas->redraw();
	}
}

void VisualizationController::SetCanvas( Fl_Smart_Tile* canvas )
{
	mCanvas = canvas;
}
