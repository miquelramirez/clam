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

#include "SegmentView.hxx"
#include "ErrGUI.hxx"


using CLAM::Enum;
using CLAM::Segment;
using CLAM::Frame;
using CLAM::Spectrum;
using CLAM::TData;
using CLAM::TTime;
using CLAM::DataArray;

using namespace CLAMGUI;

// Services declaration

Enum::tEnumValue ProcDataView<Segment>::Services::sEnumValues[] =
	{
		{ ProcDataView<Segment>::Services::GetTFView,   "GetTFView" },
		{ ProcDataView<Segment>::Services::GetDuration, "GetDuration" },
		{ 0, NULL }
	};

Enum::tValue ProcDataView<Segment>::Services::sDefault = ProcDataView<Segment>::Services::GetTFView;

void ProcDataView<Segment>::Release()
{
	mTFView.Release();

	mObservedObj = NULL;

	mDurationGetter = NULL;

	UnlockModel();
	UnlockServices();

}

void ProcDataView<Segment>::BindTo( Segment* obj )
{
	CLAM_ASSERT( obj != NULL, "Provided object was null" );

	mObservedObj = obj;

	mTFView.BindTo( obj );

	LockModel();
}

void ProcDataView<Segment>::Configure(Aspect& cfg )
{

	GenCb* cb_holder = NULL;

	UnlockServices();

	bool someone = false;

	cb_holder = cfg.GetRequest( Services("GetDuration") );

	someone |= Acknowledge( mDurationGetter, cb_holder );

	cb_holder = cfg.GetRequest( Services("GetTFView") );

	if ( cb_holder != NULL )
	{
		ExpandRequest( mTFView, cb_holder );
		someone = true;
	}
	// Delegated services

	if (someone)
		LockServices();
}

void ProcDataView<Segment>::Refresh()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{

		if ( mDurationGetter != NULL )
		{
			(*mDurationGetter)( mObservedObj->GetEndTime() - mObservedObj->GetBeginTime() );
		}

		mTFView.Refresh();
	}
}
