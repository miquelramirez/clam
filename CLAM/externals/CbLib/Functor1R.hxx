/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __FUNCTOR1R__
#define __FUNCTOR1R__

#include "FunctorBase.hxx"

namespace CBL
{

template < class TypeOfReturn, class Parm1Type >
class Functor1R : public FunctorBase
{
public:

	Functor1R( DummyInit * = 0 )
	{
	}
	
	TypeOfReturn operator()( Parm1Type param1 ) const
	{
		return thunk( *this, param1 );
	}
	
	FunctorBase::operator bool;
	
protected:
	
	typedef TypeOfReturn (*Thunk) (const FunctorBase&, Parm1Type );
	
	Functor1R( Thunk t, const void* c, const void* f, size_t sz )
		: FunctorBase( c, f, sz), thunk ( t )
	{
	}

private:

	Thunk thunk;

};

// Translators

// From non-member function

template < class TypeOfReturn, class Parm1Type, class Func >
class FunctionTranslator1R : public Functor1R< TypeOfReturn, Parm1Type >
{
public:

	FunctionTranslator1R( Func f )
		: Functor1R< TypeOfReturn, Parm1Type >( thunk, NULL, (void*)f, size_t(0))
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1 )
	{
		return (Func( ftor.func )) ( param1 );
	}

};

// From member Function

template < class TypeOfReturn, class Parm1Type, class Callee, class MemFunc >
class MemberTranslator1R : public Functor1R< TypeOfReturn, Parm1Type >
{
public:

	MemberTranslator1R( Callee& c, MemFunc mf )
		: Functor1R< TypeOfReturn, Parm1Type > ( thunk, &c, &mf, sizeof( MemFunc ) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1 )
	{
	
		Callee* callee = (Callee*) ftor.callee;
#ifdef _MSC_VER

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		return (callee->*mfref)(param1);
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		
		return (callee->*memFunc)( param1 );
#endif
	}
};

// "specialized" makeFunctor function for pointers to non-member or static functions

template < class TypeOfReturn, class Parm1Type, class TTypeOfReturn, class TParm1Type >
inline FunctionTranslator1R< TypeOfReturn, Parm1Type,  TTypeOfReturn (*)(TParm1Type) >
makeFunctor ( Functor1R< TypeOfReturn, Parm1Type >*, TTypeOfReturn (*f) (TParm1Type) )
{
	return FunctionTranslator1R< TypeOfReturn, Parm1Type, TTypeOfReturn (*)(TParm1Type) >( f );
}

// "specialized" makeFunctor function to pointer to non-const member functions

template < class TypeOfReturn, class Parm1Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type >
inline MemberTranslator1R< TypeOfReturn, Parm1Type, Callee, TTypeOfReturn (CallType::*) (TParm1Type) >
makeFunctor ( Functor1R< TypeOfReturn, Parm1Type >*, Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type) )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type);
	
	return MemberTranslator1R< TypeOfReturn, Parm1Type, Callee, MemFunc >( c, f );
}

// "specialized" makeFunctor function to pointer to const member functions

template < class TypeOfReturn, class Parm1Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type >
inline MemberTranslator1R< TypeOfReturn, Parm1Type, const Callee, TTypeOfReturn (CallType::*) (TParm1Type) const >
makeFunctor ( Functor1R< TypeOfReturn, Parm1Type >*, const Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type) const )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type) const;
	
	return MemberTranslator1R< TypeOfReturn, Parm1Type, const Callee, MemFunc >( c, f );
}


}

#endif // Functor1R.hxx




