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

#include "InControlView.hxx"
#include "ErrGUI.hxx"
#include "DataTypes.hxx"
using CLAM::TData;
using namespace CLAMGUI;

// Service definition

Enum::tEnumValue InControlView::Services::sEnumValues[] =
{
	{ InControlView::Services::Apply_Control, "Apply_Control" },
	{ 0, NULL }
};

Enum::tValue InControlView::Services::sDefault = InControlView::Services::Apply_Control;

// Class definition

void InControlView::Release()
{
	mSignalGetter = NULL;
	mLastValue = 0.0f;
	
	
	UnlockModel();
	UnlockServices();
	mMustDoUpdate = false;
}

void InControlView::Configure( Aspect& cfg )
{
	UnlockServices();	
	
	GenCb*   cb_holder;
	
	cb_holder = cfg.GetRequest( Services("Apply_Control") );

	if ( Acknowledge( mSignalGetter, cb_holder ) )
		{
			LockServices();
		}
	
}

void InControlView::BindTo( Processing* obj, unsigned int control )
{
	
	CLAM_ASSERT( obj!=NULL, "Object was null!!!" );
	
	mObservedObj = obj;
	mControlID = control;
	
	LockModel();
}

void InControlView::Refresh()
{
	
}

void InControlView::UpdateModel()
{
	if ( mMustDoUpdate && HasModelLocked() && HasServicesLocked() )
	{
		TData value_to_send = (*mSignalGetter)();

		if ( mSignalGetter != NULL )
			mObservedObj->DoControl( mControlID, value_to_send );

		mLastValue = value_to_send;
		mMustDoUpdate = false;

	}
}

void InControlView::NotifyUpdate()
{
	if ( HasServicesLocked() && HasModelLocked() )
	{
		TData temp = (*mSignalGetter)();
		
		if ( temp!= mLastValue )
		{
			mMustDoUpdate = true;
			mLastValue = temp;
			
		}
	}
}
