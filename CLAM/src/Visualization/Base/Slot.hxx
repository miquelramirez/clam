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

#ifndef __SLOT__
#define __SLOT__

#include "Connection.hxx"
#include <list>

namespace CLAMGUI
{

/**
 * An Slot is something you may connect with a signal.
 * @see Signal
 */
	class Slot
	{
		typedef std::list<Connection>        tConnectionList;
		typedef tConnectionList::iterator    tConnectionIterator;
  
	public:
  
		void Bind(const Connection& conn );
  
		void Unbind();

		void Unbind( Connection::tConnectionId conn );
  
		unsigned  ActiveConnections() const
		{
			return mActiveConnections.size();
		}
  
		~Slot();
  
	private:
  
		tConnectionList     mActiveConnections;
	};

}

#endif // Slot.hxx
