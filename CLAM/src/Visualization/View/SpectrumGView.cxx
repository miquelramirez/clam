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

#include "SpectrumGView.hxx"
using CLAM::SpectrumConfig;
using CLAM::SpecTypeFlags;

using namespace CLAMGUI;

// Services definition

Enum::tEnumValue ProcDataView<Spectrum>::Services::sEnumValues[] =
{
	{ ProcDataView<Spectrum>::Services::GetMag, "GetMag" },
	{ ProcDataView<Spectrum>::Services::GetPhase, "GetPhase" },
	{ ProcDataView<Spectrum>::Services::GetSpectralRange, "GetSpectralRange" },
	{ ProcDataView<Spectrum>::Services::GetSpectralResolution, "GetSpectralResolution" },
	{ ProcDataView<Spectrum>::Services::GetScale, "GetScale" },
	{ 0, NULL }
};

Enum::tValue ProcDataView<Spectrum>::Services::sDefault = ProcDataView<Spectrum>::Services::GetMag;

// View implementation

void ProcDataView<Spectrum>::Configure( Aspect& cfg )
{
	
	GenCb* cb_holder;
	
	UnlockServices();
	
	// Starting configuration code
	bool someone = false;
	cb_holder = cfg.GetRequest( Services("GetMag") );
	someone |= Acknowledge( RenderMagnitude, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetPhase") );
	
	someone |= Acknowledge( RenderPhase, cb_holder );
	cb_holder = cfg.GetRequest( Services("GetSpectralResolution") );
	someone |= Acknowledge( RenderSpectralResolution, cb_holder );
	//Size rendering is not mandatory for this view
	cb_holder = cfg.GetRequest( Services("GetSpectralRange") );
	someone |= Acknowledge( RenderSpectralRange, cb_holder );
	
	cb_holder = cfg.GetRequest( Services("GetScale" ) );
	someone |= Acknowledge( RenderScale, cb_holder );
	if ( someone )
	{
		LockServices();
	}
}

void ProcDataView<Spectrum>::Refresh()
{
	if ( HasServicesLocked() && HasModelLocked() )
	{
		SpectrumConfig myCfg;
		
		mObservedObj->GetConfig( myCfg );
		
		CLAM_BEGIN_CHECK
		SpecTypeFlags flags = myCfg.GetType();
		if ( !flags.bMagPhase ) // Something strange happens
		{
			CLAM_ASSERT( mObservedObj->HasMagBuffer(), "No Magnitude buffer Dynamic Attribute in the Spectrum to be rendered" );
			CLAM_ASSERT( mObservedObj->HasPhaseBuffer(), "No Phase buffer Dynamic Attribute in the Spectrum to be rendered" );
			mObservedObj->Debug();
			CLAM_ASSERT( false, "Spectrum MisConfigured, Check the Debug Info ( Debug.xml ) ");
		}
		CLAM_END_CHECK


		// Refreshing code
		if ( RenderScale != NULL )
			(*RenderScale) ( myCfg.GetSize(), myCfg.GetScale() );
		if ( RenderSpectralRange != NULL )
			(*RenderSpectralRange ) (myCfg.GetSpectralRange() );
		if ( RenderSpectralResolution != NULL )
			( *RenderSpectralResolution ) ( TData(  myCfg.GetSpectralRange() / myCfg.GetSize() ) );
		if ( RenderMagnitude != NULL ) 
			(*RenderMagnitude)( mObservedObj->GetMagBuffer() );
		if ( RenderPhase != NULL )
			(*RenderPhase) ( mObservedObj->GetPhaseBuffer() );
		
	}
}

void ProcDataView<Spectrum>::Release()
{
	RenderMagnitude = NULL;
	RenderPhase = NULL;
	RenderSpectralRange = NULL;
	RenderSpectralResolution = NULL;
	RenderScale = NULL;
	
	mObservedObj = NULL;
	
	UnlockServices();
	UnlockModel();
}
