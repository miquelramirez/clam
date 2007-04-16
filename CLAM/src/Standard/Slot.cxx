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

#include "Slot.hxx"
#include <algorithm>

namespace SigSlot
{

	class ConnectionSearchPred
	{
	public:

		ConnectionSearchPred( Connection::tConnectionId id )
			: mSoughtID( id )
		{
		}

		bool operator()( const Connection& conn )
		{
			return ( conn.GetID() == mSoughtID );
		}

	private:

		Connection::tConnectionId mSoughtID;
	};

	Slot::~Slot()
	{
		Unbind();
	}

	void Slot::Unbind()
	{
		mActiveConnections.clear();
	}

	void Slot::Bind( const Connection& conn )
	{
		mActiveConnections.push_back( conn );
	}

	void Slot::Unbind( Connection::tConnectionId conn )
	{
		tConnectionIterator i = std::find_if(	mActiveConnections.begin(), 
							mActiveConnections.end(), 
							ConnectionSearchPred( conn ) );

		mActiveConnections.erase( i );
	}

}

