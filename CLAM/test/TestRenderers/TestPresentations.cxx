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

#include "TestPresentations.hxx"
#include <FL/Fl_Double_Window.H>
#include "GLPortNew.hxx"
#include "GLLinearSpRenderer.hxx"
#include "GLLogSpRenderer.hxx"
#include "GLLogPowSpRenderer.hxx"
#include "GLPhaseSpRenderer.hxx"
#include "FLDisplayContainer.hxx"
#include "ViewConfiguration.hxx"
#include "SpectrumGView.hxx"
#include "AudioGView.hxx"
#include "GeometryKit.hxx"
using CLAMGUI::GLPort;
using CLAMGUI::GLLinearSpRenderer;
using CLAMGUI::GLLogSpRenderer;
using CLAMGUI::GLLogPowSpRenderer;
using CLAMGUI::GLPhaseSpRenderer;
using CLAMGUI::Aspect;
using CLAMGUI::Viewport;
using CLAM::GLState;
using CLAM::TData;
using CLAM::DataArray;
using CLAM::FLDisplayContainer;
using namespace CLAMGUITest;

TestPhasePresentation::~TestPhasePresentation()
{
	delete mWindow;
}

void TestPhasePresentation::Do()
{
	 mPort->redraw();
}


void TestPhasePresentation::LinkWithView( View* view )
{
	mLinkedView = view;
	PublishCallbacks();
}

void TestPhasePresentation::PublishCallbacks()
{
	Aspect my_aspect;

	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetPhase"), &mPhaseDataCb );
	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetSpectralRange"), &mRangeCb );
	
	mLinkedView->Configure( my_aspect );
}

void TestPhasePresentation::Show()
{
	mWindow->show();
}

void TestPhasePresentation::UpdatePhaseData( const DataArray& array )
{
	Viewport view_specs;
	
	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top , -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void TestPhasePresentation::UpdateSpectralRangeData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void TestPhasePresentation::Init( const Geometry& g)
{

	// FLTK thingies initialization

	mWindow = new Fl_Double_Window( g.GetX(), g.GetW(), g.GetW(), g.GetH(), "Displaying the phase" );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );
	

	mPort = new GLPort( CLAMGUI::Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLPhaseSpRenderer( 0, 200, 0 );
	
	mDispContainer->Add( *mPort );

	mWindow->resizable( mDispContainer );
	
	mWindow->end();

	// Linking the port and the renderer...


	mPort->Configure( new GenericGLState , CBL::makeFunctor( (CBL::Functor0*)0, *mRenderer, &GLPhaseSpRenderer::Draw) );

	// Initializing the callback placeholders
	
	mPhaseDataCb = CBL::makeFunctor( (CBL::Functor1< const DataArray& >*)0, *this, &TestPhasePresentation::UpdatePhaseData );
	mRangeCb = CBL::makeFunctor( (CBL::Functor1< TData >*)0, *this, &TestPhasePresentation::UpdateSpectralRangeData );
	
}

// TestLinearMag implementation

TestLinearMagPresentation::~TestLinearMagPresentation()
{
	delete mWindow;
}

void TestLinearMagPresentation::Do()
{
	 mPort->redraw();
}

void TestLinearMagPresentation::LinkWithView( View* view )
{
	mLinkedView = view;
	PublishCallbacks();
}

void TestLinearMagPresentation::PublishCallbacks()
{
	Aspect my_aspect;

	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetMag"), &mMagDataCb );
	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetSpectralRange"), &mRangeCb );
	
	mLinkedView->Configure( my_aspect );
}

void TestLinearMagPresentation::Show()
{
	mWindow->show();
}

void TestLinearMagPresentation::UpdateMagData( const DataArray& array )
{
	Viewport view_specs;

	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top , -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void TestLinearMagPresentation::UpdateSpectralRangeData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void TestLinearMagPresentation::Init( const Geometry& g )
{

	// FLTK thingies initialization
	
	mWindow = new Fl_Double_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), "Displaying the mag linearly" );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );
	

	mPort = new GLPort( CLAMGUI::Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLLinearSpRenderer( 0, 200, 0 );
	
	mDispContainer->Add( *mPort );
	
	mWindow->resizable( mDispContainer );
	
	mWindow->end();

	// Linking the port and the renderer...
	
	mPort->Configure( new GenericGLState , CBL::makeFunctor( (CBL::Functor0*)0, *mRenderer, &GLLinearSpRenderer::Draw) );
	
	// Initializing the callback placeholders
	
	mMagDataCb = CBL::makeFunctor( (CBL::Functor1< const DataArray& >*)0, *this, &TestLinearMagPresentation::UpdateMagData );
	mRangeCb = CBL::makeFunctor( (CBL::Functor1< TData >*)0, *this, &TestLinearMagPresentation::UpdateSpectralRangeData );
	
}

// TestLogPow implementation

TestLogPowPresentation::~TestLogPowPresentation()
{
	delete mWindow;
}

void TestLogPowPresentation::Do()
{
	mPort->redraw();
}

void TestLogPowPresentation::LinkWithView( View* view )
{
	mLinkedView = view;
	PublishCallbacks();
}

void TestLogPowPresentation::PublishCallbacks()
{
	Aspect my_aspect;

	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetMag"), &mMagDataCb );
	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetSpectralRange"), &mRangeCb );
	
	mLinkedView->Configure( my_aspect );
}

void TestLogPowPresentation::Show()
{
	mWindow->show();
}

void TestLogPowPresentation::UpdateMagData( const DataArray& array )
{
	Viewport view_specs;

	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top , -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void TestLogPowPresentation::UpdateSpectralRangeData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void TestLogPowPresentation::Init( const Geometry& g )
{

	// FLTK thingies initialization

	mWindow = new Fl_Double_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), "Displaying Spectrum Power" );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );

	mPort = new GLPort( CLAMGUI::Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLLogPowSpRenderer( 0, 200, 0 );
	
	mDispContainer->Add( *mPort );
	
	mWindow->resizable( mDispContainer );

	mWindow->end();

	// Linking the port and the renderer...

	mPort->Configure( new GenericGLState , CBL::makeFunctor( (CBL::Functor0*)0, *mRenderer, &GLLogSpRenderer::Draw) );

	// Initializing the callback placeholders

	mMagDataCb = CBL::makeFunctor( (CBL::Functor1< const DataArray& >*)0, *this, &TestLogPowPresentation::UpdateMagData );
	mRangeCb = CBL::makeFunctor( (CBL::Functor1< TData >*)0, *this, &TestLogPowPresentation::UpdateSpectralRangeData );
	
}

// TestLogMag implementation

TestLogMagPresentation::~TestLogMagPresentation()
{
	delete mWindow;
}

void TestLogMagPresentation::Do()
{
	mPort->redraw();
}

void TestLogMagPresentation::LinkWithView( View* view )
{
	mLinkedView = view;
	PublishCallbacks();
}

void TestLogMagPresentation::PublishCallbacks()
{
	Aspect my_aspect;

	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetMag"), &mMagDataCb );
	my_aspect.AddCallback(CLAMGUI::ProcDataView<Spectrum>::Services("GetSpectralRange"), &mRangeCb );
	
	mLinkedView->Configure( my_aspect );
}

void TestLogMagPresentation::Show()
{
	mWindow->show();
}

void TestLogMagPresentation::UpdateMagData( const DataArray& array )
{
	Viewport view_specs;

	mRenderer->DefineViewport( array, view_specs );

	mDispContainer->SetHorRange( view_specs.left, view_specs.right );
	mDispContainer->SetVerRange( view_specs.top , -( view_specs.top - view_specs.bottom ) );
	mDispContainer->mpHorRuler->mInteger = view_specs.isIntX;
	mDispContainer->mpVerRuler->mInteger = view_specs.isIntY;

	mRenderer->CacheData( array );
}

void TestLogMagPresentation::UpdateSpectralRangeData( TData value )
{
	mRenderer->SetSamplingRate( value );
}

void TestLogMagPresentation::Init( const Geometry& g )
{

	// FLTK thingies initialization

	mWindow = new Fl_Double_Window( 100, 100, 640, 400, "Displaying Logarithmically" );
	mDispContainer = new FLDisplayContainer( 0, 0, 640, 400 );
	
	mPort = new GLPort( CLAM::Rect<int>( 0, 0, 640, 400 ) );
	mRenderer = new GLLogSpRenderer( 0, 200, 0 );

	mWindow = new Fl_Double_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), "Displaying Logarithmically" );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );

	mPort = new GLPort( CLAMGUI::Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLLogSpRenderer( 0, 200, 0 );

	mWindow->resizable( mDispContainer );

	mWindow->end();

	// Linking the port and the renderer...

	mPort->Configure( CLAM::Rect<float>(0.0f, 1024.0f, 3.14159f, 0.0f ),  new GenericGLState , CBL::makeFunctor( (CBL::Functor0*)0, *mRenderer, &GLLogSpRenderer::Draw) );

	mPort->Configure( new GenericGLState , CBL::makeFunctor( (CBL::Functor0*)0, *mRenderer, &GLLogSpRenderer::Draw) );

	mMagDataCb = CBL::makeFunctor( (CBL::Functor1< const DataArray& >*)0, *this, &TestLogMagPresentation::UpdateMagData );
	mRangeCb = CBL::makeFunctor( (CBL::Functor1< TData >*)0, *this, &TestLogMagPresentation::UpdateSpectralRangeData );

}
