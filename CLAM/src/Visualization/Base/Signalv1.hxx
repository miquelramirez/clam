#ifndef __SIGNALV1__
#define __SIGNALV1__

#include "Functor1.hxx"
#include "Signal.hxx"
#include "Slot.hxx"
#include "CallbackHandler.hxx"
#include <list>

namespace CLAMGUI
{

template < typename ParmType1 >
	class Signalv1 : public Signal
{
public:
	typedef CBL::Functor1<ParmType1>                  tCallbackType;
	typedef CallbackHandler<Signalv1<ParmType1> >     tSuperType;

public:
	
	template < class RefType, typename PtrMember >
		Slot Connect( RefType thisRef, PtrMember pMember )
	{
		Slot s( AssignSlot(), this );

		mSuper.AddCallback( s.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember ) );

		return s;
	}

	template < typename PtrFunction >
		Slot Connect( PtrFunction pMember )
	{
		Slot s( AssignSlot(), this );

		mSuper.AddCallback( s.GetID(), CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember ) );

		return s;
	}
	
	void Emit( ParmType1 parm )
	{
		if ( mSuper.HasNoCallbacks() )
			return;
		
		tSuperType::tCallList calls = mSuper.GetCalls();
		tSuperType::tCallList::iterator i = calls.begin();
		tSuperType::tCallList::iterator end = calls.end();

		while ( i != end )
			{
				(*(*i))( parm );
				i++;
			}
		
	}

	void FreeSlot( Slot* pSlot )
	{
		mSuper.RemoveCall( pSlot->GetID() );
		FreeSlotId( pSlot->GetID() );
	}
private:
	tSuperType  mSuper;
};

}

#endif // Signalv1.hxx
