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

#include "SegmentTFView.hxx"
#include "List.hxx"
#include "ErrGUI.hxx"
#include "GlobalEnums.hxx"
using CLAM::EScale;

using CLAM::Enum;
using CLAM::Segment;
using CLAM::Frame;
using CLAM::Spectrum;
using CLAM::TData;
using CLAM::TTime;
using CLAM::DataArray;


using namespace CLAMGUI;

// Services declaration

Enum::tEnumValue SegmentTFView::Services::sEnumValues[] =
	{
		{ SegmentTFView::Services::GetTFData, "GetTFData" },
		{ SegmentTFView::Services::GetTFParameters, "GetTFParameters" },
		{ 0, NULL }
	};

Enum::tValue SegmentTFView::Services::sDefault = SegmentTFView::Services::GetTFData;

void SegmentTFView::Release()
{
	mObservedObj = NULL;

	mFramesGetter = NULL;
	mParametersGetter = NULL;

	UnlockServices();
	UnlockModel();
}

void SegmentTFView::BindTo( Segment* obj )
{
	CLAM_ASSERT( obj != NULL, "Provided object was null" );

	mObservedObj = obj;

	LockModel();
}

void SegmentTFView::Configure( Aspect& cfg )
{
	GenCb* cb_holder = NULL;

	UnlockServices();

	bool someone = false;

	cb_holder = cfg.GetRequest( Services("GetTFData") );

	someone |= Acknowledge( mFramesGetter, cb_holder );
	
	cb_holder = cfg.GetRequest( Services("GetTFParameters") );

	someone |= Acknowledge( mParametersGetter, cb_holder );
 
	if ( someone )
	{
		LockServices();
	}

}

void SegmentTFView::Refresh()
{
	if ( HasModelLocked() && HasServicesLocked() )
	{
		if ( mParametersGetter == NULL || mFramesGetter == NULL )
			throw( ErrGUI("SegmentTFView is an 'all-or-nothing' view" ) );

		bool isLin = false; 
		TData spec_range = mObservedObj->GetFrame(0).GetSpectrum().GetSpectralRange();
		unsigned int num_frames = mObservedObj->GetnFrames();
		unsigned int idx = 0;

		(*mParametersGetter) ( mObservedObj->GetBeginTime(), mObservedObj->GetEndTime(), spec_range, num_frames );
	 

		while ( idx < num_frames )
		{
			CLAM::Frame& f = mObservedObj->GetFrame(int(idx));
			isLin = f.GetSpectrum().GetScale() == EScale::eLinear;
			(*mFramesGetter)( f.GetSpectrum().GetMagBuffer(), isLin );
			idx++;
		}
	}
}
