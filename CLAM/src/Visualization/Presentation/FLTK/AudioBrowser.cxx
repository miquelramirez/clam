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

#include "AudioBrowser.hxx"

using namespace CLAMGUI;

AudioBrowser::AudioBrowser( int X, int Y, int W, int H, const char* label )
{

	Geometry g( X, Y, W, H );

	Init( g, label );
}

AudioBrowser::AudioBrowser( const Geometry& g, const char* label )
{
	Init( g, label );
}

AudioBrowser::~AudioBrowser()
{

}

void AudioBrowser::PublishCallbacks()
{
	Aspect my_aspect;
	ProcDataView<Audio>::Services s;
	
	s = ProcDataView<Audio>::Services::GetBuffer;
	my_aspect.AddRequest( s, &mDrawBufferCb );
	s = ProcDataView<Audio>::Services::GetSampleRate;
	my_aspect.AddRequest( s, &mDrawSampleRateCb );
	s = ProcDataView<Audio>::Services::GetDuration;
	my_aspect.AddRequest( s, &mDrawDurationCb );
	
	mLinkedView->Configure( my_aspect );
}

void AudioBrowser::Show()
{
	mWindow->show();
}

void AudioBrowser::UpdateBufferData( const DataArray& array )
{
	Viewport view_specs;

	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top, -fabs(view_specs.top-view_specs.bottom));//-( view_specs.top - view_specs.bottom ) );
//	mDispContainer->SetVerRange( view_specs.top, view_specs.bottom );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void AudioBrowser::UpdateSampleRateData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void AudioBrowser::UpdateDurationData( TData value )
{

}

void AudioBrowser::Init( const Geometry& g, const char* l )
{
	// FLTK thingies initialization

	mWindow = new PresentationWindow( this , g.GetX(), g.GetY(), g.GetW(), g.GetH(), l );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );


	mGLsurface = new GLPortSigSlot( Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLWaveRenderer( 0, 200, 0 );

	mDispContainer->Add( mGLsurface );

	mWindow->resizable( mDispContainer );

	mWindow->end();

	// Linking the port and the renderer...

	mGLsurface->SetCullingCallback( makeMemberFunctor4( float, float, unsigned, unsigned, *mRenderer, GLWaveRenderer, PerformCulling ) );
	mGLsurface->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
	mGLsurface->Configure( new AudioPresGLState, makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );

	// Initializing the callback placeholders

	mDrawBufferCb = makeMemberFunctor1( const DataArray&, *this, AudioBrowser, UpdateBufferData );
	mDrawDurationCb = makeMemberFunctor1( TData, *this, AudioBrowser, UpdateDurationData );
	mDrawSampleRateCb = makeMemberFunctor1( int, *this, AudioBrowser, UpdateSampleRateData );

}
