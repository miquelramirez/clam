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

#ifndef __SIGNALV0IMPLVC6__
#define __SIGNALV0IMPLVC6__

#ifndef __SIGNALV0__
#error "This is an internal implementation header. You are not allowed to include it directly!"
#endif

namespace SigSlot
{

	class Signalv0 
		: public Signal
	{
	public:
		typedef CBL::Functor0     tCallbackType;
		// Begin of ConnectionHandler
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
  
		typedef tCallbackType*                                     tCallbackPtr;
		typedef std::list<tCallbackPtr>                            tCallList;
		typedef std::list<tCallbackPtr >::iterator        tCallIterator;
		typedef std::list<tCallback>                               tCallbackList;
		typedef std::list<tCallback>::iterator            tCbListIterator;
		typedef std::list<tCallback>::const_iterator      const_tCbListIterator;
  
  
	protected:		
  
		void AddCallback( tConnectionId pConnection, Slot* slot, tCallbackType cb )
		{
			mCallbacks.push_back( tCallback( pConnection, slot, cb ) );
		}
  
		bool HasNoCallbacks( ) const
		{
			return mCallbacks.empty();
		}
  
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
  
		void DestroyConnections()
		{
			tCbListIterator elem;
    
			while ( !mCallbacks.empty() )
			{
				elem = mCallbacks.begin();
	
				elem->mSlot->Unbind( elem->mConnection );
			}
		}
		// End of "ConnectionHandler"
	public:
		virtual ~Signalv0()
		{
			DestroyConnections();
		}
  
  
		void Connect( Slotv0& slot )
		{
			Connection c ( AssignConnection(), this );
    
			AddCallback( c.GetID(), &slot, slot.GetMethod() );
    
			slot.Bind(c);
		}
  
		void Emit()
		{
			if ( HasNoCallbacks() )
				return;
    
			tCallList calls = GetCalls();
			tCallIterator i = calls.begin();
			tCallIterator end = calls.end();
    
			while( i != end )
			{
				(*(*i))();
				i++;
			}
		}
  
		void FreeConnection( Connection* pConnection )
		{
			RemoveCall( pConnection->GetID() );
			FreeConnectionId( pConnection->GetID() );
		}

	private:
  
		tCallList       mCalls;
		tCallbackList   mCallbacks;
  
  
	};

}

#endif // Signalv0ImplVC6.hxx

