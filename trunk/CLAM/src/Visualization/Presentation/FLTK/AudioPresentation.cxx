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

#include "CBL.hxx"
#include "AudioPresentation.hxx"
#include "GeometryKit.hxx"
#include "Viewport.hxx"
#include <algorithm>

using std::max_element;
using std::min_element;
using namespace CLAMGUI;



void ProcDataPresentation<Audio>::PublishCallbacks()
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

void ProcDataPresentation<Audio>::Show()
{
	mWindow->show();
}

void ProcDataPresentation<Audio>::UpdateBufferData( const DataArray& array )
{
	Viewport view_specs;

	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top, -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void ProcDataPresentation<Audio>::UpdateSampleRateData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void ProcDataPresentation<Audio>::UpdateDurationData( TData value )
{

}

void ProcDataPresentation<Audio>::Init( const Geometry& g, const char* l )
{
	// FLTK thingies initialization

	mWindow = new PresentationWindow( this , g.GetX(), g.GetY(), g.GetW(), g.GetH(), l );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );


	mPort = new GLPort( Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLWaveRenderer( 0, 200, 0 );

	mDispContainer->Add( mPort );

	mWindow->resizable( mDispContainer );

	mWindow->end();

	// Linking the port and the renderer...

	mPort->SetCullingCallback( makeMemberFunctor4( float, float, unsigned, unsigned, *mRenderer, GLWaveRenderer, PerformCulling ) );
	mPort->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );
	mPort->Configure( new AudioPresGLState, makeMemberFunctor0( *mRenderer, GLWaveRenderer, Draw) );

	// Initializing the callback placeholders

	mDrawBufferCb = makeMemberFunctor1( const DataArray&, *this, ProcDataPresentation<Audio>, UpdateBufferData );
	mDrawDurationCb = makeMemberFunctor1( TData, *this, ProcDataPresentation<Audio>, UpdateDurationData );
	mDrawSampleRateCb = makeMemberFunctor1( int, *this, ProcDataPresentation<Audio>, UpdateSampleRateData );

}
