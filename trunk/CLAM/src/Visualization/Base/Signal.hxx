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

#ifndef __SIGNAL__
#define __SIGNAL__

#include <utility>
#include <map>
#include <stack>

namespace CLAMGUI
{

	class Connection;

/**
 * A signal represents an event to be signaled to a group of Slots.
 *
 * This class is an abstract one, it only defines the common interface
 * for signals and some common implementation.
 */

	class Signal
	{
	public:
		typedef unsigned  tConnectionId;
  
		virtual ~Signal(){}
  
		/**
		 * Undoes the given Conection
		 */
		virtual void FreeConnection( Connection* ) = 0;
  
	protected:
		void FreeConnectionId( tConnectionId );
		tConnectionId AssignConnection();
  
		// Types
		typedef std::stack< unsigned >      tConnectionIdStack;
	protected:
  
		static tConnectionIdStack  smFreeIdStack;
		static tConnectionId       smLastConnectionId;
	};

}

#endif // Signal.hxx
