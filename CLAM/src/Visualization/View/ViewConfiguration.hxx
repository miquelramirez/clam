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

#ifndef __VIEWCONFIGURATION__
#define __VIEWCONFIGURATION__

#include <map>
#include <string>
#include "FunctorBase.hxx"
#include "Enum.hxx"
using CLAM::Enum;
using std::string;

namespace CLAMGUI
{

	class Aspect
	{
		typedef std::map< string , CBL::FunctorBase* > Services;
		
	public:
		
		void AddRequest( const Enum& service_requested, CBL::FunctorBase* signal )
			{
				mcfgTable[ service_requested.GetString() ] = signal;
			}
		
		CBL::FunctorBase* GetRequest( const Enum& service_acknowledged )
			{
				Services::iterator i = mcfgTable.find( service_acknowledged.GetString() );
				
				if ( i == mcfgTable.end() )
					return NULL;
				else
					return i->second;
			}
		
	private:
		
		Services mcfgTable;
		
	};
	
}

#endif // ViewConfiguration.hxx
