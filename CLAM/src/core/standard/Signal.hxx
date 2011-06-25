/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

namespace SigSlot
{

	class Connection;

/** \class Signal
 *  \brief Abstract 'Event Dispatcher'
 *
 * Sometimes, is very useful that when an object is subject of a certain message,
 * or state change, must notify this change to a number of listeners. This situation,
 * albeit it can be modeled quite neatly in OOP suffers a major drawback: coupling. Coupling
 * tipically risks the possibility of reusing a certain object due to the fact that the caller
 * must know to some extent the callee interface.
 *
 * However there exist some solutions to this known problem. One of them is the 'Signal and Slot'
 * idiom, which features an extremely low coupling between callers and callees. For more details,
 * take a look on the CLAM Signal & Slot Library.
 *
 * This class is an abstract one, already implementing common behaviour, the signal
 * and slot connection handling issues.
 */
	class Signal
	{
	public:
		typedef std::stack< unsigned >      tConnectionIdStack;
		typedef unsigned  tConnectionId;
  
		/** \brief Destructor
		 *
		 *  Class destructor
		 */
		virtual ~Signal();
 
		/** \brief Removes a connection
		 *
		 *  Removes the given connections, so the Signal does not
		 *  notify anymore the Slot behind that Connection.
		 *  \param conn The connection to be freed
		 */
		virtual void FreeConnection( Connection* conn ) = 0;
  
	protected:
		void FreeConnectionId( tConnectionId );
		tConnectionId AssignConnection();
  
	protected:
  
		static tConnectionId       smLastConnectionId;
	};

}

#endif // Signal.hxx

