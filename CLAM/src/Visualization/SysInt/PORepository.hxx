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

#ifndef __POREPOSITORY__
#define __POREPOSITORY__

#include "Component.hxx"
#include "Processing.hxx"
#include <map>
#include <list>
#include <string>
#include <cstring>

namespace CLAMGUI
{

	struct Cmp_OnType
	{
		const char* tag;

		Cmp_OnType( const char* t )
			: tag ( t )
		{
		}

		bool operator()( CLAM::Processing* obj )
		{
			return ( strcmp( obj->GetClassName(), tag ) == 0 );
		}
	};

	struct Cmp_OnName
	{
		std::string label;

		Cmp_OnName( const char* t )
			: label( t )
		{
		}

		bool operator()( CLAM::Processing* obj )
		{
			return ( obj->GetName() == label );
		}

	};

	class PORepository : public CLAM::Component
	{
	public:
		typedef std::list< CLAM::Processing* >  POList;
		typedef std::map< std::string, POList > POLib;
		typedef std::list< std::string >        NameList;

	public:

		/**
		 *   It would be nice to store the system composition somewhere
		 *   wouldn't it?
		 */

		void StoreOn( CLAM::Storage& store )
		{

		}

		void LoadFrom( CLAM::Storage& store )
		{

		}

		/**
		 *   This method triggers repository initialization to store
		 *   the current composition of the system
		 */
		void AcquireSystem();

		const NameList& GetClassNames();

		const POList& GetAll();

		const POList& GetThese( const char* classname );

		CLAM::Processing* GetThisOne( const char* poname );

		void AddPO( CLAM::Processing* po_obj );

		void RemovePO( CLAM::Processing* po_obj );

		void RemovePO( const char* classname, const char* po_obj_name );

		void RefreshRepository();

		virtual ~PORepository()
		{
		}

	protected:

		void RemovePO( const char* classname, const Cmp_OnName& pred );

		void FlushRepository();

	private:

		POLib          mPOSys;
		POList         mLastQuery;
		NameList       mClasses;
	};

	inline std::istream & operator >> ( std::istream& os, PORepository& po_rep )
	{
		return os;
	}

	inline std::ostream& operator << ( std::ostream& os, PORepository& po_rep )
	{
		return os;
	}


}

#endif // PORepository.hxx
