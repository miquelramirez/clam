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

#include "SegmentTFPresentation.hxx"
using namespace CLAMGUI;

void SegmentTFPresentation::PublishCallbacks()
{
	Aspect cfg;

	cfg.AddRequest( SegmentTFView::Services("GetTFData"), &mDrawFrame );
	cfg.AddRequest( SegmentTFView::Services("GetTFParameters"), &mCatchParams );

	mLinkedView->Configure( cfg );

}

void SegmentTFPresentation::Show()
{
	mWindow->show();
	mGLContextAvailable = true;
}

void SegmentTFPresentation::Init( const Geometry& g, const char* label )
{
// FLTK things init

	mWindow = new Fl_Double_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );

	mPort = new GLPort( Rect<int>( 0, 0, g.GetW(), g.GetH() ) );

	mRenderer = new GLTF2DRenderer();

	mDispContainer->Add( mPort );

	mWindow->resizable( mDispContainer );

	mWindow->end();

	// Let's link the port and the renderer

	mPort->Configure( new SegmentTFGLState, makeMemberFunctor0( *mRenderer, GLTF2DRenderer, Draw ) );

	// Callbacks placeholders setup

	mDrawFrame = makeMemberFunctor2( const DataArray&, bool, *this, SegmentTFPresentation, CatchThoseFrames );

	mCatchParams = makeMemberFunctor4( TTime, TTime, TData, unsigned int, *this, SegmentTFPresentation, CatchParams );

}

void SegmentTFPresentation::CatchThoseFrames( const DataArray& array, bool isLin )
{
	if ( mPort == NULL || !mGLContextAvailable )
	{
		throw( GUIException( "No OpenGL context available for issuing OpenGL commands" ) );
	}
	else
	{
		mPort->make_current(); // We activate the OpenGL context
		mRenderer->RenderFrame( array, isLin );
	}
}

void SegmentTFPresentation::CatchParams( TTime btime, TTime etime, TData specrng, unsigned int nframes )
{
	Viewport view_specs;

	mRenderer->SetupRender( btime, etime, specrng, nframes );
	mRenderer->DefineViewport( view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top , -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

}
