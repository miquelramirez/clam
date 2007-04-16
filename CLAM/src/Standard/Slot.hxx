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

#ifndef __SLOT__
#define __SLOT__

#include "Connection.hxx"
#include <list>

namespace SigSlot
{

/** \class Slot
 *  \brief The receiver of a Signal
 *
 *  If the Signal represents the concept of Event Notifying, the Slot represents
 *  the 'Event handler'
 *
 *  \see Signal
 */
		class Slot
		{
				typedef std::list<Connection>        tConnectionList;
				typedef tConnectionList::iterator    tConnectionIterator;
				
		public:
		
				/** \brief Binds the Slot with a Signal
				 *
				 *  This method keeps the Slot bound to a given Signal ( identified by a
				 *  Connection ).
				 *
				 *  \param conn The connection to establish
				 */
				void Bind(const Connection& conn );
				
				/** \brief Disconnects the Slot
				 *
				 *  Calling this method severs all the existing connections between
				 *  the present slot and any signal.
				 */
				void Unbind();
				
				/** \brief Disconnects the Slot
				 *
				 *  Calling this method severs the connection between the Slot and the
				 *  Signal identified by the connection Id number.
				 *  \param conn The connection to be severed.
				 */
				void Unbind( Connection::tConnectionId conn );
				
				/** \brief Tells how many signals are connected
				 *
				 *  This method tells the client how many connections are already established
				 *  with Signals for the present Slot.
				 *  \return The number of Signals connected to the Slot
				 */
				unsigned  ActiveConnections() const
				{
						return mActiveConnections.size();
				}
				
				/** \brief Destructor
				 *
				 *  Destructor method.
				 */
				~Slot();
				
		private:
				
				tConnectionList     mActiveConnections;
		};
		
}

#endif // Slot.hxx

