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

#include "SpectrumPresentation.hxx"
#include "GLLogSpRenderer.hxx"
using CLAM::DB;
using namespace CLAMGUI;

void ProcDataPresentation<Spectrum>::PublishCallbacks()
{
	Aspect my_aspect;
	
	
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetMag"), &mDrawMagCb );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetPhase"), &mDrawPhaseCb );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetSpectralRange"), &mDrawRangeCb );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetSpectralResolution"), &mDrawResolutionCb );
	my_aspect.AddRequest( ProcDataView<Spectrum>::Services("GetScale"), &mDrawScaleCb );
	
	mLinkedView->Configure( my_aspect );
}

void ProcDataPresentation<Spectrum>::Show()
{
	mWindow->show();
}

void ProcDataPresentation<Spectrum>::UpdateMagData( const DataArray& array )
{
	
	// MRJ: We only need to really update the view when the spectrum metrics change
	
	if ( !mSpectralMetrixLocked )
		{
			// We estimate a max equal to number of spectrum bins

			// :MRJ: We discard any estimation. this is a ugly hack that
			// whose need I hopefully will vanquish soon
			mSpecPresMetrix.top_es = false;
			mRenderer->DefineViewport( array, mSpecPresMetrix );
			
			mDispContainer->SetHorRange( mSpecPresMetrix.left, mSpecPresMetrix.right );
			mDispContainer->SetVerRange( 20 , -120 );
			mDispContainer->mpHorRuler->mInteger = mSpecPresMetrix.isIntX;
			mDispContainer->mpVerRuler->mInteger = mSpecPresMetrix.isIntY;
			mSpectralMetrixLocked = true;
		}
	mRenderer->CacheData( array );
}

void ProcDataPresentation<Spectrum>::UpdatePhaseData( const DataArray& array )
{
	
}

void ProcDataPresentation<Spectrum>::UpdateSpectralRangeData( TData value )
{
	
	if ( mRenderer->GetSamplingRate() != value )
		{
			mRenderer->SetSamplingRate( value );
			mSpectralMetrixLocked = false;
		}
	

}

void ProcDataPresentation<Spectrum>::UpdateSpectralResolutionData( TData value )
{
	static TData last_value = 0;
	
	if ( last_value != value )
		{
			last_value = value;
			mSpectralMetrixLocked = false;
		}
}

void ProcDataPresentation<Spectrum>::UpdateScale( unsigned int nbins, EScale scale )
{
	if ( scale.GetValue() == EScale::eLinear )
		{
			mSpecPresMetrix.SetTopEstimation( TData( nbins ) );
			mSpecPresMetrix.SetBottomEstimation( TData( 0 ) );
		}
	else if ( scale.GetValue() == EScale::eLog )
		{
			//	mSpecPresMetrix.SetTopEstimation( DB( TData(nbins), CLAM_DB_SCALING ) );
			mSpecPresMetrix.SetTopEstimation( 0 );
			mSpecPresMetrix.SetBottomEstimation( -100 );
		}
	else
		CLAM_ASSERT( false, "Spectrum was crippled somehow since it hasn't provided a valid scale" );
}

void ProcDataPresentation<Spectrum>::Init( const Geometry& g, const char* label)
{
	
	// FLTK thingies initialization
	
	mWindow = new PresentationWindow( this, g.GetX(), g.GetY(), g.GetW(), g.GetH(), label );
	mDispContainer = new FLDisplayContainer( 0, 0, g.GetW(), g.GetH() );
	
	mPort = new GLPort( Rect<int>( 0, 0, g.GetW(), g.GetH() ) );
	mRenderer = new GLLogSpRenderer( 0, 200, 0 );
	
	mDispContainer->Add( mPort );
	
	mWindow->resizable( mDispContainer );
	
	mWindow->end();
	
	// Linking the port and the renderer...

//	mPort->SetCullingCallback( makeMemberFunctor4( float, float, unsigned,unsigned, 
//												   *mRenderer, GLLinearSpRenderer, PerformCulling ) );


	mPort->SetDrawingCallback( makeMemberFunctor0( *mRenderer, GLLinearSpRenderer, Draw ) );

	mPort->Configure( new SpecPresGLState , makeMemberFunctor0( *mRenderer, GLLinearSpRenderer, Draw ) );
	
	// Initializing the callback placeholders
	mDrawMagCb = makeMemberFunctor1( const DataArray&, *this, ProcDataPresentation<Spectrum>, UpdateMagData );
	mDrawPhaseCb = makeMemberFunctor1( const DataArray&, *this, ProcDataPresentation<Spectrum>, UpdatePhaseData );
	mDrawRangeCb = makeMemberFunctor1( TData, *this, ProcDataPresentation<Spectrum>, UpdateSpectralRangeData );
	mDrawResolutionCb = makeMemberFunctor1( TData, *this, ProcDataPresentation<Spectrum>, UpdateSpectralResolutionData );
	
	
	// Initializing the callback placeholders
	mDrawMagCb = makeMemberFunctor1( const DataArray&, *this, ProcDataPresentation<Spectrum>, UpdateMagData );
	mDrawPhaseCb = makeMemberFunctor1( const DataArray&, *this, ProcDataPresentation<Spectrum>, UpdatePhaseData );
	mDrawRangeCb = makeMemberFunctor1( TData, *this, ProcDataPresentation<Spectrum>, UpdateSpectralRangeData );
	mDrawResolutionCb = makeMemberFunctor1( TData, *this, ProcDataPresentation<Spectrum>, UpdateSpectralResolutionData );
	mDrawScaleCb = makeMemberFunctor2( unsigned int, EScale, *this, ProcDataPresentation<Spectrum>, UpdateScale );

}
