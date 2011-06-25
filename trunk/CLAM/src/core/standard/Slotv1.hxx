/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

