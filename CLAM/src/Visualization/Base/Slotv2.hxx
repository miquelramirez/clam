#ifndef __SLOTV2__
#define __SLOTV2__

#include "Slot.hxx"
#include "CBL.hxx"
#include "Assert.hxx"

namespace SigSlot
{

template < typename ParmType1, typename ParmType2 >
class Slotv2 : public Slot
{
		typedef CBL::Functor2<ParmType1, ParmType2> WrappedFuncType;

public:

		Slotv2()
				: mIsInit( false )
		{
		}

		virtual ~Slotv2()
		{
		}

		template < class RefType, typename PtrMember >
		void Wrap( RefType thisRef, PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor2<ParmType1,ParmType2>*)0, *thisRef, pMember );
				mIsInit = true;
		}

		template <  typename PtrMember >
		void Wrap( PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor2<ParmType1,ParmType2>*)0, pMember );
				mIsInit = true;
		}


		const WrappedFuncType& GetMethod() const
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				return mFunctor;
		}

		void operator()( ParmType1 parm1, ParmType2 parm2 )
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				mFunctor( parm1, parm2 );
		}

private:

		WrappedFuncType   mFunctor;
		bool              mIsInit;

};

}

#endif // SlottedMethod.hxx
