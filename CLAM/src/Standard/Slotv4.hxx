#ifndef __SLOTV4__
#define __SLOTV4__

#include "Slot.hxx"
#include "CBL.hxx"
#include "Assert.hxx"

namespace SigSlot
{

template < typename ParmType1, typename ParmType2, typename ParmType3, typename ParmType4 >
class Slotv4 : public Slot
{
		typedef CBL::Functor4<ParmType1, ParmType2, ParmType3, ParmType4> WrappedFuncType;

public:

		Slotv4()
				: mIsInit( false )
		{
		}

		virtual ~Slotv4()
		{
		}

		template < class RefType, typename PtrMember >
		void Wrap( RefType thisRef, PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor4<ParmType1,ParmType2,ParmType3,ParmType4>*)0, *thisRef, pMember );
				mIsInit = true;
		}

		template <  typename PtrMember >
		void Wrap( PtrMember pMember )
		{
				mFunctor = CBL::makeFunctor( (CBL::Functor4<ParmType1,ParmType2,ParmType3,ParmType4>*)0, pMember );
				mIsInit = true;
		}


		const WrappedFuncType& GetMethod() const
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				return mFunctor;
		}

		void operator()( ParmType1 parm1, ParmType2 parm2, ParmType3 parm3, ParmType4 parm4 )
		{
				CLAM_ASSERT( mIsInit, "Must be initialized" );
				mFunctor( parm1, parm2, parm3, parm4 );
		}

private:

		WrappedFuncType   mFunctor;
		bool              mIsInit;

};

}

#endif // SlottedMethod.hxx
