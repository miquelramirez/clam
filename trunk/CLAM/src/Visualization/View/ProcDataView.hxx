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

#ifndef __PROCDATAVIEW__
#define __PROCDATAVIEW__

#include "GView.hxx"
#include "ViewConfiguration.hxx"
#include "Storage.hxx"

using CLAM::Storage;


namespace CLAMGUI
{


template < typename ProcDataType >
class ProcDataView 
	: public View
{
	typedef CBL::Functor0R< Storage& >    StorageCb;

public:

	ProcDataView()
	{
	}

	virtual ~ProcDataView()
	{
	}

	virtual const char* GetClassName() const
	{
		return "GenericPDView";
	}

	virtual void Refresh()
	{
		// :TODO: Not implemented yet
		if ( HasModelLocked() && HasServicesLocked() )
			{			
				mObservedObj->StoreOn( (*mStorageSignal)() );
			}
	}

	virtual void Configure( Aspect& a )
	{
		GenCb* cb_holder = NULL;
		
		UnlockServices();
		
		cb_holder = a.GetRequest( View::GenericServices( "XMLDump" ) );
		
		if ( Acknowledge( mStorageSignal, cb_holder) )
			LockServices();
	}

	virtual void BindTo( ProcDataType* obj )
	{
		CLAM_ASSERT( obj!=NULL, "Model Object was NULL!!!!" );
		
		mObservedObj = obj;
		LockModel();
	}

	virtual void Release()
	{
		UnlockModel();
		UnlockServices();
		
		mObservedObj = NULL;
		mStorageSignal = NULL;
	}

private:

	ProcDataType*     mObservedObj;
	StorageCb*        mStorageSignal;

};


}

#endif // ProcDataView.hxx
