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

#ifndef __FUNCTOR4R__
#define __FUNCTOR4R__

#include "FunctorBase.hxx"

namespace CBL
{

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type >
class Functor4R : public FunctorBase
{
public:

	Functor4R( DummyInit * = 0 )
	{
	}
	
	TypeOfReturn operator()( Parm1Type param1, Parm2Type param2, Parm3Type param3, Parm4Type param4 ) const
	{
		return thunk( *this, param1, param2, param3, param4 );
	}
	
	FunctorBase::operator bool;
	
protected:
	
	typedef TypeOfReturn (*Thunk) (const FunctorBase&, Parm1Type, Parm2Type, Parm3Type, Parm4Type );
	
	Functor4R( Thunk t, const void* c, const void* f, size_t sz )
		: FunctorBase( c, f, sz), thunk ( t )
	{
	}

private:

	Thunk thunk;

};

// Translators

// From non-member function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type, class Func >
class FunctionTranslator4R : public Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >
{
public:

	FunctionTranslator4R( Func f )
		: Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >( thunk, NULL, (void*)f, size_t(0))
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3, Parm4Type param4 )
	{
		return (Func( ftor.func )) ( param1, param2, param3, param4 );
	}
	
};

// From member Function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type, class Callee, class MemFunc >
class MemberTranslator4R : public Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >
{
public:

	MemberTranslator4R( Callee& c, MemFunc mf )
		: Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type > ( thunk, &c, &mf, sizeof( MemFunc ) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3, Parm4Type param4 )
	{
	
		Callee* callee = (Callee*) ftor.callee;

#ifdef _MSC_VER

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		return (callee->*mfref)(param1,param2,param3,param4);
		
#else
		MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );

		return (callee->*memFunc)( param1, param2, param3, param4);

#endif	
	}
};

// "specialized" makeFunctor function for pointers to non-member or static functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type, class TTypeOfReturn, class TParm1Type, class TParm2Type, class TParm3Type, class TParm4Type >
inline FunctionTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, TTypeOfReturn (*)(TParm1Type, TParm2Type, TParm3Type, TParm4Type ) >
makeFunctor ( Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >*, TTypeOfReturn (*f) (TParm1Type, TParm2Type, TParm3Type, TParm4Type ) )
{
	return FunctionTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, TTypeOfReturn (*)(TParm1Type, TParm2Type, TParm3Type, TParm4Type ) >( f );
}

// "specialized" makeFunctor function to pointer to non-const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type, class TParm4Type >
inline MemberTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type, TParm3Type, TParm4Type) >
makeFunctor ( Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >*, Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type, TParm3Type, TParm4Type) )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type, TParm3Type, TParm4Type);
	
	return MemberTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, Callee, MemFunc >( c, f );
}

// "specialized" makeFunctor function to pointer to const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Parm3Type, class Parm4Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type, class TParm4Type >
inline MemberTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, const Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type, TParm3Type, TParm4Type) const >
makeFunctor ( Functor4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type >*, const Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type, TParm3Type, TParm4Type ) const )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type, TParm3Type, TParm4Type ) const;
	
	return MemberTranslator4R< TypeOfReturn, Parm1Type, Parm2Type, Parm3Type, Parm4Type, const Callee, MemFunc >( c, f );
}

}

#endif // Functor4R.hxx


