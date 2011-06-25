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

#include "Connection.hxx"
#include "Signal.hxx"

namespace SigSlot
{

	Connection::Connection()
		: mMustFreeSignal(false), mConnectedSignal( NULL )
	{
	}

	Connection::~Connection()
	{
		if ( mMustFreeSignal )
			mConnectedSignal->FreeConnection( this );
	}

	Connection::Connection( tConnectionId id, Signal* connSig )
		: mMustFreeSignal( true ), mID( id ), mConnectedSignal( connSig )
	{
	}

	Connection::Connection( const Connection& c )
	{
		mMustFreeSignal = c.mMustFreeSignal;
		mID = c.mID;
		mConnectedSignal = c.mConnectedSignal;
		c.mMustFreeSignal = false;
	}

	Connection& Connection::operator=( Connection& s )
	{
		mMustFreeSignal = s.mMustFreeSignal;
		mID = s.mID;
		mConnectedSignal = s.mConnectedSignal;
		s.mMustFreeSignal = false;

		return *this;
	}

}

