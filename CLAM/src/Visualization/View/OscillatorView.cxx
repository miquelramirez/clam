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

#include "OscillatorGView.hxx"
using namespace CLAMGUI;
using CLAM::Oscillator;
using CLAM::OscillatorConfig;
#include "ErrGUI.hxx"


// Services declaration

Enum::tEnumValue ProcObjView<Oscillator>::Services::sEnumValues[] =
	{
		{ ProcObjView<Oscillator>::Services::GetPitch, "GetPitch" },
		{ ProcObjView<Oscillator>::Services::GetAmp, "GetAmp" },
		{ ProcObjView<Oscillator>::Services::GetModIdx, "GetModIdx" },
		{ ProcObjView<Oscillator>::Services::GetPhase, "GetPhase" },
		{ ProcObjView<Oscillator>::Services::SetPitch, "SetPitch" },
		{ ProcObjView<Oscillator>::Services::SetAmp, "SetAmp" },
		{ ProcObjView<Oscillator>::Services::SetModIdx, "SetModIdx" },
		{ ProcObjView<Oscillator>::Services::SetPhase, "SetPhase" },
		{ 0, NULL }
	};

Enum::tValue ProcObjView<Oscillator>::Services::sDefault = ProcObjView<Oscillator>::Services::GetPitch;

// OscillatorView declaration

void ProcObjView< Oscillator >::Release()
{
	RenderFreq = NULL;
	RenderAmp = NULL;
	RenderModIndex = NULL;
	RenderPhase = NULL;

	mFreqView.Release();
	mAmpView.Release();
	mModIdxView.Release();
	mPhaseView.Release();

	mObservedObj = NULL;

	UnlockModel();
	UnlockServices();
}

void ProcObjView< Oscillator >::Configure( Aspect& cfg )
{
	GenCb* cb_holder = NULL;

	UnlockServices();

	// Configuration code

	bool someone = false;

	// Oscillator pitch rendering service configuration

	cb_holder = cfg.GetRequest( Services("GetPitch") );

	someone |= Acknowledge( RenderFreq, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetAmp" ) );

	someone |= Acknowledge( RenderAmp, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetModIdx") );

	someone |= Acknowledge( RenderModIndex, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetPhase") );

	someone |= Acknowledge( RenderPhase, cb_holder );


	cb_holder = cfg.GetRequest( Services("SetPitch") );

	DelegateRequest( mFreqView, InControlView::Services("Apply_Control"), cb_holder ); 

	cb_holder = cfg.GetRequest( Services("SetAmp") );

	DelegateRequest( mAmpView, InControlView::Services("Apply_Control"), cb_holder );

	cb_holder = cfg.GetRequest( Services("SetModIdx") );

	DelegateRequest( mModIdxView, InControlView::Services("Apply_Control"), cb_holder );

	cb_holder = cfg.GetRequest( Services("SetPhase") );

	DelegateRequest( mPhaseView, InControlView::Services( "Apply_Control" ), cb_holder );

	LockServices();
}

void ProcObjView< Oscillator >::Refresh()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		const OscillatorConfig& osc_cfg = dynamic_cast< const OscillatorConfig& > ( mObservedObj->GetConfig() );

		if ( RenderFreq != NULL )
		{
			( *RenderFreq ) ( osc_cfg.GetFrequency() );
		}
		if ( RenderAmp != NULL )
		{
			( *RenderAmp ) ( osc_cfg.GetAmplitude() );
		}

		if ( RenderModIndex != NULL )
		{
			( *RenderModIndex ) ( osc_cfg.GetModIndex() );
		}

		if ( RenderPhase != NULL )
		{
			( *RenderPhase ) ( osc_cfg.GetPhase() );
		}
	}
}

void ProcObjView< Oscillator >::UpdateModel()
{

	if ( HasModelLocked() && HasServicesLocked() )
	{
		mFreqView.UpdateModel();

		mAmpView.UpdateModel();

		mModIdxView.UpdateModel();

		mPhaseView.UpdateModel();
	}
}


