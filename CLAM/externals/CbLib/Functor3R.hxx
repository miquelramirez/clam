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

#ifndef __FUNCTOR3R__
#define __FUNCTOR3R__

#include "FunctorBase.hxx"

namespace CBL
{

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type >
class Functor3R : public FunctorBase
{
public:

	Functor3R( DummyInit * = 0 )
	{
	}
	
	TypeOfReturn operator()( Parm1Type param1, Parm2Type param2, Parm3Type param3 ) const
	{
		return thunk( *this, param1, param2, param3 );
	}
	
	FunctorBase::operator bool;
	
protected:
	
	typedef TypeOfReturn (*Thunk) (const FunctorBase&, Parm1Type, Parm2Type, Parm3Type );
	
	Functor3R( Thunk t, const void* c, const void* f, size_t sz )
		: FunctorBase( c, f, sz), thunk ( t )
	{
	}

private:

	Thunk thunk;

};

// Translators

// From non-member function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Func >
class FunctionTranslator3R : public Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >
{
public:
	
	FunctionTranslator3R( Func f )
		: Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >( thunk, NULL, (void*)f, size_t(0))
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3 )
	{
		return (Func( ftor.func )) ( param1, param2, param3 );
	}
	
};

// From member Function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Callee, class MemFunc >
class MemberTranslator3R : public Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >
{
public:

	MemberTranslator3R( Callee& c, MemFunc mf )
		: Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type > ( thunk, &c, &mf, sizeof( MemFunc ) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3 )
	{
	
		Callee* callee = (Callee*) ftor.callee;
#ifdef _MSC_VER_

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		return (callee->*mfref)(param1,param2,param3);
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		
		return (callee->*memFunc)( param1, param2, param3);
#endif
	}
};

// "specialized" makeFunctor function for pointers to non-member or static functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class TTypeOfReturn, class TParm1Type, class TParm2Type, class TParm3Type >
inline FunctionTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, TTypeOfReturn (*)(TParm1Type, TParm2Type, TParm3Type ) >
makeFunctor ( Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >*, TTypeOfReturn (*f) (TParm1Type, TParm2Type, TParm3Type) )
{
	return FunctionTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, TTypeOfReturn (*)(TParm1Type, TParm2Type, TParm3Type) >( f );
}

// "specialized" makeFunctor function to pointer to non-const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type >
inline MemberTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type, TParm3Type) >
makeFunctor ( Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >*, Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type, TParm3Type) )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type, TParm3Type);
	
	return MemberTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Callee, MemFunc >( c, f );
}

// "specialized" makeFunctor function to pointer to const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type >
inline MemberTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, const Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type, TParm3Type) const >
makeFunctor ( Functor3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type >*, const Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type, TParm3Type) const )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type, TParm3Type) const;
	
	return MemberTranslator3R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, const Callee, MemFunc >( c, f );
}



}

#endif // Functor3R.hxx


