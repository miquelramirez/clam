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

#include <iostream>
#include "FDFilterGenView.hxx"
using CLAM::FDFilterGen;
using CLAM::FDFilterGenConfig;
using std::cout;
using std::endl;
#include "ErrGUI.hxx"


using namespace CLAMGUI;

// Services definition

Enum::tEnumValue ProcObjView<FDFilterGen>::Services::sEnumValues[] =
{
	{ ProcObjView<FDFilterGen>::Services::GetGain, "GetGain" },
	{ ProcObjView<FDFilterGen>::Services::GetHighCutoff, "GetHighCutoff" },
	{ ProcObjView<FDFilterGen>::Services::GetLowCutoff, "GetLowCutoff" },
	{ ProcObjView<FDFilterGen>::Services::GetPassBandSlope, "GetPassBandSlope" },
	{ ProcObjView<FDFilterGen>::Services::GetStopBandSlope, "GetStopBandSlope" },
	{ ProcObjView<FDFilterGen>::Services::GetName, "GetName" },
	{ ProcObjView<FDFilterGen>::Services::SetGain, "SetGain" },
	{ ProcObjView<FDFilterGen>::Services::SetHighCutoff, "SetHighCutoff" },
	{ ProcObjView<FDFilterGen>::Services::SetLowCutoff, "SetLowCutoff" },
	{ ProcObjView<FDFilterGen>::Services::SetPassBandSlope, "SetPassBandSlope" },
	{ ProcObjView<FDFilterGen>::Services::SetStopBandSlope, "SetStopBandSlope" },
	{ 0, NULL }
};

Enum::tValue ProcObjView<FDFilterGen>::Services::sDefault = ProcObjView<FDFilterGen>::Services::GetGain;

// FDFilterGenView Definition

void ProcObjView< FDFilterGen >::Release()
{
	
	RenderGain = NULL;
	RenderLowCutoff = NULL;
	RenderHighCutoff = NULL;
	RenderPassBandSlope = NULL;
	RenderStopBandSlope = NULL;
	RenderName = NULL;

	mGainCtlView.Release();
	mLCCtlView.Release();
	mHCCtlView.Release();
	mPBSCtlView.Release();
	mSBSCtlView.Release();
	
	mObservedObj = NULL;
	
	UnlockModel();
	UnlockServices();
}

void ProcObjView< FDFilterGen >::Configure( Aspect& cfg )
{
	GenCb* cb_holder;
	
	UnlockServices();
	
	// Configuration code
	
	bool someone = false;
	
	// Filter Gain rendering service configuration
	
	cb_holder = cfg.GetRequest( Services("GetGain") );
	
	someone |= Acknowledge( RenderGain, cb_holder );


	// Filter Lower Cutoff freq rendering service configuration

	cb_holder = cfg.GetRequest( Services("GetLowCutoff") );

	someone |= Acknowledge( RenderLowCutoff, cb_holder ) ;

	// Filter Higher Cutoff freq rendering service configuration

	cb_holder = cfg.GetRequest( Services("GetHighCutoff") );

	someone |= Acknowledge( RenderHighCutoff, cb_holder );

	// Filter Pass Band Slope rendering service configuration

	cb_holder = cfg.GetRequest( Services("GetPassBandSlope") );

	someone |= Acknowledge( RenderPassBandSlope, cb_holder );

	// Filter Pass Band Slope rendering service configuration

	cb_holder = cfg.GetRequest( Services("GetStopBandSlope") );

	someone |= Acknowledge( RenderStopBandSlope, cb_holder );

	// Filter name Rendering Services

	cb_holder = cfg.GetRequest( Services("GetName") );

	someone |= Acknowledge( RenderName, cb_holder );

	// Filter gain Acquiring services

	cb_holder = cfg.GetRequest( Services("SetGain") );

	DelegateRequest( mGainCtlView, InControlView::Services("Apply_Control"), cb_holder );

	// Filter low cutoff acquiring services

	cb_holder = cfg.GetRequest( Services("SetLowCutoff") );

	DelegateRequest( mLCCtlView, InControlView::Services("Apply_Control"), cb_holder );

	// Filter high cutoff acquiring services

	cb_holder = cfg.GetRequest( Services("SetHighCutoff") );

	DelegateRequest( mHCCtlView, InControlView::Services("Apply_Control"), cb_holder );

	// Filter pass band slope services

	cb_holder = cfg.GetRequest( Services("SetPassBandSlope") );

	DelegateRequest( mPBSCtlView, InControlView::Services("Apply_Control"), cb_holder );

	// Filter stop band slope services

	cb_holder = cfg.GetRequest( Services("SetStopBandSlope") );

	DelegateRequest( mSBSCtlView, InControlView::Services("Apply_Control"), cb_holder );

	LockServices();
}

void ProcObjView< FDFilterGen >::Refresh()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		const FDFilterGenConfig& filter_cfg = dynamic_cast< const FDFilterGenConfig& > ( mObservedObj->GetConfig() );

		if ( RenderGain!= NULL )
		{
			( *RenderGain )( filter_cfg.GetGain() );
		}

		if ( RenderLowCutoff != NULL )
		{
			( *RenderLowCutoff )( filter_cfg.GetLowCutOff() );
		}

		if ( RenderHighCutoff != NULL )
		{
			( *RenderHighCutoff )( filter_cfg.GetHighCutOff() );
		}

		if ( RenderPassBandSlope != NULL )
		{
			( *RenderPassBandSlope )( filter_cfg.GetPassBandSlope() );
		}

		if ( RenderStopBandSlope != NULL )
		{
			( *RenderStopBandSlope )( filter_cfg.GetStopBandSlope() );
		}

		if ( RenderName != NULL )
		{
			( *RenderName )( mObservedObj->GetName() );
		}
	} 
}

void ProcObjView< FDFilterGen >::UpdateModel()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		mGainCtlView.UpdateModel();
		mLCCtlView.UpdateModel();
		mHCCtlView.UpdateModel();
		mPBSCtlView.UpdateModel();
		mSBSCtlView.UpdateModel();
	}
}

