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

#ifndef __SLOTV0__
#define __SLOTV0__

#include "Slot.hxx"
#include "CBL.hxx"
//#include "Assert.hxx"

namespace SigSlot
{

	class Slotv0 : public Slot
	{
		typedef CBL::Functor0 WrappedFuncType;
  
	public:
  
		Slotv0()
			: mIsInit( false )
		{
		}

		virtual ~Slotv0()
		{
		}
  
		template < class RefType, typename PtrMember >
		void Wrap( RefType thisRef, PtrMember pMember )
		{
			mFunctor = CBL::makeFunctor( (CBL::Functor0*)0, *thisRef, pMember );
			mIsInit = true;
		}
  
		template <  typename PtrMember >
		void Wrap( PtrMember pMember )
		{
			mFunctor = CBL::makeFunctor( (CBL::Functor0*)0, pMember );
			mIsInit = true;
		}
  

		const WrappedFuncType& GetMethod() const
		{
			//CLAM_ASSERT( mIsInit, "Must be initialized" );
			return mFunctor;
		}
  
		void operator()( )
		{
			//CLAM_ASSERT( mIsInit, "Must be initialized" );
			mFunctor(  );
		}
  
	private:
  
		WrappedFuncType   mFunctor;
		bool              mIsInit;
  
	};

}

#endif // Slotv0.hxx

