#ifndef __CALLBACKHANDLER__
#define __CALLBACKHANDLER__

#include <list>
#include <utility>
#include "Slot.hxx"

namespace CLAMGUI
{

template < class SignalType >
class CallbackHandler
{
public:
	typedef typename SignalType::tCallbackType                 tCallbackType;
	typedef typename SignalType::tSlotId                       tSlotId;
	typedef std::pair<tSlotId, tCallbackType>                  tCallback;
	typedef std::list<tCallbackType* >                         tCallList;
	typedef std::list<tCallback>                               tCallbackList;
	typedef typename std::list<tCallback>::iterator            tCbListIterator;
	typedef typename std::list<tCallback>::const_iterator      const_tCbListIterator;

public:

	void AddCallback( tSlotId pSlot, tCallbackType cb )
	{
		mCallbacks.push_back( tCallback( pSlot, cb ) );
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
				mCalls.push_back( &(i->second) );
				i++;
			}
		
		return mCalls;
	}

	void RemoveCall(  tSlotId id )
	{
		tCbListIterator i = mCallbacks.begin();
		tCbListIterator end = mCallbacks.end();

		while ( i!=end )
			{
				if ( i->first == id )
					{
						mCallbacks.erase( i );
						break;
					}
				i++;
			}
	}

private:
	
	tCallList       mCalls;
	tCallbackList   mCallbacks;
	
};

}

#endif
