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

#ifndef __CONNECTION__
#define __CONNECTION__

namespace CLAMGUI
{

	class Signal;

	class Connection
	{
	public:
		typedef unsigned tConnectionId;

		Connection();
		Connection( tConnectionId id, Signal* connectedSignal );
		Connection& operator=( Connection& s );
		Connection( const Connection& s );

		tConnectionId GetID() const
		{
			return mID;
		}

		~Connection();

	private:
		mutable bool  mMustFreeSignal;
		tConnectionId mID;
		Signal*       mConnectedSignal;
	};

}

#endif // Connection.hxx
