#ifndef __SLOTV1__
#define __SLOTV1__

#include "Slot.hxx"
#include "Functor1.hxx"
#include "Assert.hxx"

namespace SigSlot
{

	template < typename ParmType1 >
	class Slotv1 : public Slot
	{
		typedef CBL::Functor1<ParmType1> WrappedFuncType;

	public:

		Slotv1()
			: mIsInit( false )
		{
		}

		virtual ~Slotv1()
		{
		}

		template < class RefType, typename PtrMember >
		void Wrap( RefType thisRef, PtrMember pMember )
		{
			mFunctor = CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, *thisRef, pMember );
			mIsInit = true;
		}

		template <  typename PtrMember >
		void Wrap( PtrMember pMember )
		{
			mFunctor = CBL::makeFunctor( (CBL::Functor1<ParmType1>*)0, pMember );
			mIsInit = true;
		}


		const WrappedFuncType& GetMethod() const
		{
			CLAM_ASSERT( mIsInit, "Must be initialized" );
			return mFunctor;
		}

		void operator()( ParmType1 parm )
		{
			CLAM_ASSERT( mIsInit, "Must be initialized" );
			mFunctor( parm );
		}

	private:

		WrappedFuncType   mFunctor;
		bool              mIsInit;

	};

}

#endif // SlottedMethod.hxx
