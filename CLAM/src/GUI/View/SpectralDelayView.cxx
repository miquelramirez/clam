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

#include "SpectralDelayView.hxx"
#include "ErrGUI.hxx"
using namespace CLAMGUI;

Enum::tEnumValue ProcObjView< SpectralDelay >::Services::sEnumValues[] =
{
	{ ProcObjView<SpectralDelay>::Services::GetDelay, "GetDelay" },
	{ ProcObjView<SpectralDelay>::Services::GetMaxDelay, "GetMaxDelay" },
	{ ProcObjView<SpectralDelay>::Services::SetDelay, "SetDelay" },
	{ ProcObjView<SpectralDelay>::Services::SetMaxDelay, "SetMaxDelay" },
	{ 0, NULL }
};

Enum::tValue ProcObjView<SpectralDelay>::Services::sDefault = ProcObjView<SpectralDelay>::Services::GetDelay;


void ProcObjView< SpectralDelay >::UpdateModel()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		mDelayCtlView.UpdateModel();
	}
}

void ProcObjView< SpectralDelay >::Refresh()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		if ( mDelayGetter != NULL )
		{
			( *mDelayGetter )( mObservedObj->RealDelay() );
		} 
		if ( mMaxDelayGetter != NULL )
		{
			CLAM::TokenDelayConfig objconf = dynamic_cast< const CLAM::TokenDelayConfig& >(mObservedObj->GetConfig());
			(*mMaxDelayGetter )( objconf.GetMaxDelay() );
		}
	}
}

void ProcObjView< SpectralDelay >::Release()
{

	mDelayCtlView.Release();
	mDelayGetter = NULL;
	mMaxDelayGetter = NULL;


	mObservedObj = NULL;

	UnlockServices();
	UnlockModel();
}

void ProcObjView< SpectralDelay >::Configure( Aspect& cfg )
{
	UnlockServices();

	bool someone = false;

	GenCb* cb_holder = cfg.GetRequest( Services("GetDelay") );

	someone |= Acknowledge( mDelayGetter, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetMaxDelay") );

	someone |= Acknowledge( mMaxDelayGetter, cb_holder );

	cb_holder = cfg.GetRequest( Services("SetDelay") );

	if ( cb_holder != NULL )
	{

		DelegateRequest( mDelayCtlView, InControlView::Services("Apply_Control"), cb_holder );

		someone = true;
	}

	if ( someone )
		LockServices();
}

void ProcObjView< SpectralDelay >::BindTo( SpectralDelay* obj )
{
	CLAM_ASSERT( obj != NULL, "Model object to be observed is NULL!!!");

	mObservedObj = obj;
	mDelayCtlView.BindTo( obj, ETokenDelayControls::delay );

	LockModel();
}
