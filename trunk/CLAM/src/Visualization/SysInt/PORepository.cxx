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

#include "TopLevelProcessing.hxx"
#include "PORepository.hxx"
#include "AddNRec.hxx" // Add to the repository and recur algorithm definition
#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;

using namespace CLAMGUI;

void PORepository::AcquireSystem()
{
	// ptlpo = pointer to top level processing object

	CLAM::TopLevelProcessing& ptlpo = CLAM::TopLevelProcessing::GetInstance();

	Add_and_Recurse worker( *this );

	std::for_each( ptlpo.composite_begin(), ptlpo.composite_end(), worker ); // Soooooooo nice :)

}

const PORepository::NameList& PORepository::GetClassNames()
{
	mClasses.clear();

	PORepository::POLib::iterator map_it = mPOSys.begin();

	while ( map_it!= mPOSys.end() )
	{
		mClasses.insert( mClasses.end(), std::string( (map_it->first) ) );
		map_it++;
	}

	return mClasses;
}

const PORepository::POList& PORepository::GetThese( const char* classname )
{
	mLastQuery.clear();

	PORepository::POLib::iterator i = mPOSys.find( classname );
	if ( i == mPOSys.end()  )
	{
		return mLastQuery;
	}
	else
	{
		mLastQuery.insert( mLastQuery.end(), (i->second).begin(), (i->second).end() );

		return mLastQuery;
	}
}

const PORepository::POList& PORepository::GetAll()
{
	mLastQuery.clear();

	PORepository::POLib::iterator i = mPOSys.begin();

	while ( i != mPOSys.end() )
	{
		mLastQuery.insert( mLastQuery.end(), (i->second).begin(), (i->second).end() );
		i++;
	}

	return mLastQuery;
}

CLAM::Processing* PORepository::GetThisOne( const char* poname )
{
	PORepository::POLib::iterator i = mPOSys.begin();
	bool found = false;
	PORepository::POList::iterator j;

	Cmp_OnName mypredicate( poname  );

	while ( i != mPOSys.end()  && !found )
	{
		j = std::find_if( (i->second).begin(), (i->second).end(), mypredicate );

		if ( j!= (i->second).end() )
			found = true;

		i++;
	}

	if (found )
		return *j;
	else
		return NULL;
}

void PORepository::AddPO( CLAM::Processing* po_obj )
{
	( mPOSys[ po_obj->GetClassName() ] ).push_back( po_obj );
}

void PORepository::RemovePO( const char* classname, const Cmp_OnName& pred )
{
	PORepository::POLib::iterator i = mPOSys.begin();
	bool found = false;
	PORepository::POList::iterator j;

	while ( i != mPOSys.end()  && !found )
	{

		j = std::find_if( (i->second).begin(), (i->second).end(), pred );

		if ( j!= (i->second).end() )
			found = true;
		i++;
	}

	if (found )	
	{
		( mPOSys[ classname ] ).erase( j );
	}
}

void PORepository::RemovePO( CLAM::Processing* po_obj )
{
	Cmp_OnName mypredicate( po_obj->GetName().c_str()  );

	RemovePO( po_obj->GetClassName(), mypredicate );
}

void PORepository::RemovePO( const char* classname, const char* po_obj_name )
{
	Cmp_OnName mypredicate( po_obj_name  );

	RemovePO( classname, mypredicate );

}

void PORepository::FlushRepository()
{
	mPOSys.clear();
}

void PORepository::RefreshRepository()
{
	FlushRepository();
	AcquireSystem();
}
