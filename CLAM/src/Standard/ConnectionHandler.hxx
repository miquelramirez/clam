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

/** \file ConnectionHandler.hxx
 */

#ifndef __CONNECTIONHANDLER__
#define __CONNECTIONHANDLER__

#include <list>
#include <utility>
#include "Connection.hxx"

/** \namespace SigSlot
 *  \brief     The namespace for CLAM Signals & Slot Library.
 *
 *  This namespace contains all the classes that compose CLAM Signals & Slots 
 *  Library.
 */
namespace SigSlot
{
	class Slot;

/** \class ConnectionHandler
 *  \brief Handler for a given Signal connections.
 *
 *  This helper class, as its name implies, handles the bookkeeping that the
 *  Signal has to maintain about the Slots that are to be called upon a call
 *  to the Emit method.
 *  This class is only used by the "Signal Serious Implementations" since some compilers
 *  go nuts when using derivation from template bases.
 *
 *  Note that the Handler class is templated by the Signal type that is going to use this handler,
 *  allowing to define nicely the types of the callbacks ( the Slots to be called ).
 *
 *  \sa Signalv1(SI), Signalv0(SI), Signalv2(SI), Signalv3(SI), Signalv4(SI) 
 */
	template < class SignalType >
	class ConnectionHandler
	{
	public:
		/** The callback type definition */
		typedef typename SignalType::tCallbackType                 tCallbackType;
		/** The ConnectionID type definition */
		typedef typename SignalType::tConnectionId                 tConnectionId;

		/** \struct tCallback
		 *  \brief  Inner type for ConnectionHandler.
		 *
		 *  This struct wraps a callback ( which is, albeit quite obfuscated, a good old function pointer )
		 *  attaching relevant information to it: the connection GUID that identifies the relationship between
		 *  the Signal and a Slot, a reference to the Slot object, and the callback itself.
		 *  This struct allows to compactly store the callbacks to be called by the signal in an STL container,
		 *  making trivial to implement operations such as inserting and deleting.
		 */
		struct tCallback
		{
			tConnectionId  mConnection;
			Slot*          mSlot;
			tCallbackType  mCallback;

			tCallback( tConnectionId id, Slot* slot, tCallbackType cb )
				: mConnection( id ), mSlot( slot ), mCallback( cb )
			{
			}
		};

		/** \brief Helper typedef
		 */
		typedef tCallbackType*                                     tCallbackPtr;

		/** \brief Helper typedef
		 */
		typedef std::list<tCallbackPtr>                            tCallList;

		/** \brief Helper typedef
		 */
		typedef typename std::list<tCallbackPtr >::iterator        tCallIterator;

		/** \brief Helper typedef
		 */
		typedef std::list<tCallback>                               tCallbackList;

		/** \brief Helper typedef
		 */
		typedef typename std::list<tCallback>::iterator            tCbListIterator;

		/** \brief Helper typedef
		 */
		typedef typename std::list<tCallback>::const_iterator      const_tCbListIterator;


	public:
		/** Adds a callback to the Signal callback list
		 *
		 *  \param pConnection  The connection ID corresponding to the callback
		 *  \param slot         The Slot object that has been connected to the Signal
		 *  \param cb           The callback object.
		 */
		void AddCallback( tConnectionId pConnection, Slot* slot, tCallbackType cb )
		{
			mCallbacks.push_back( tCallback( pConnection, slot, cb ) );
		}
		

		/**
		 *  Accessor for finding if there are any slots to be notified ( callbacks to be called ).
		 *  \return A boolean telling if there are any Slots connected to this Signal
		 */
		bool HasNoCallbacks( ) const
		{
			return mCallbacks.empty();
		}
		
		/** \brief Accessor to the "List of Calls"
		 *  
		 *  Given a Signal and several Slots connected to it, we define the "List of Calls", as the list
		 *  of function calls the SignalvX::Emit method has to execute in order to notify all the connected
		 *  slots. This method, returns the current "List of Calls" for the Signal.
		 *
		 *  \returns A non-const reference to the list of calls.
		 */
		tCallList& GetCalls( )
		{
			mCalls.clear();
				
			tCbListIterator i   = mCallbacks.begin();
			tCbListIterator end = mCallbacks.end();
				
			while ( i!=end)
			{
				mCalls.push_back( &(i->mCallback) );
				i++;
			}
				
			return mCalls;
		}
		
		/** \brief Removes a call from the "List of Calls".
		 *
		 *  Given a valid Connection GUID this methods searches for the connection
		 *  with this ID and removes the call from the list, so the Signal does not notify
		 *  any longer that Slot.
		 *
		 *  \param id  The GUID of the connection to be severed.
		 */
		void RemoveCall(  tConnectionId id )
		{
			tCbListIterator i = mCallbacks.begin();
			tCbListIterator end = mCallbacks.end();

			while ( i!=end )
			{
				if ( i->mConnection == id )
				{
					mCallbacks.erase( i );
					break;
				}
				i++;
			}
		}
		
		/** \brief Destroys all Signals connections.
		 *
		 *  Disconnects all the Slots bound to the Signal.
		 */
		void DestroyConnections()
		{
			tCbListIterator elem;

			while ( !mCallbacks.empty() )
			{
				elem = mCallbacks.begin();

				elem->mSlot->Unbind( elem->mConnection );
			}
		}

	private:
		
		tCallList       mCalls;      /**< The list of calls */
		tCallbackList   mCallbacks;  /**< The list of connections */ 
		
	};

}

#endif //ConnectionHandler.hxx

