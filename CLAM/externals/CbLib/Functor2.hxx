#ifndef __FUNCTOR2__
#define __FUNCTOR2__

#include "FunctorBase.hxx"

namespace CBL
{

/**
 *	Note that we are inheriting protected from FunctorBase
 *  so we don't feel so ashamed because of the previous
 *  void*'s and unions
 */

template < class Parm1Type, class Parm2Type >
class Functor2 : public FunctorBase
{
public:
	
	Functor2( DummyInit * = 0 )
	{
	}
	
	void operator()( Parm1Type param1, Parm2Type param2 ) const
	{
		thunk( *this, param1, param2 );
	}
	
	FunctorBase::operator bool;
	
protected:

	typedef void (*Thunk) (const FunctorBase&, Parm1Type, Parm2Type );
	
	Functor2( Thunk t, const void* c, const void* f, _CSTD::size_t sz )
		: FunctorBase( c, f, sz ), thunk( t )
	{
	}
	
private:

	Thunk thunk;
};
	
// Translators

// From non-member function

template < class Parm1Type, class Parm2Type, class Func > 
class FunctionTranslator2 : public Functor2< Parm1Type, Parm2Type >
{
public:

	FunctionTranslator2( Func f ) 
		: Functor2< Parm1Type, Parm2Type > ( thunk, NULL, (void*)f, _CSTD::size_t(0))
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2 )
	{
		(Func(ftor.func))( param1, param2 );
	}
};

// From member function

template < class Parm1Type, class Parm2Type, class Callee, class MemFunc >
class MemberTranslator2 : public Functor2< Parm1Type, Parm2Type >
{
public:
	
	MemberTranslator2( Callee &c, const MemFunc& m )
		: Functor2< Parm1Type, Parm2Type > (thunk, &c, &m, sizeof( MemFunc ) )
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2 )
	{
		Callee* callee = (Callee*) ftor.callee;
#ifdef __MSVC__

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		(callee->*mfref)( param1, param2 );
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		(callee->*memFunc)( param1, param2 );
#endif
	}
};

// makeFunctors helper functions

// NOTE: There must be a mean available for the compiler to do an implicit cast between
// Parm1Type and TParm1Type

// "specialized" makeFunctor function for pointer to non-member functions

template < class Parm1Type, class Parm2Type, class TypeOfReturn, class TParm1Type, class TParm2Type >
inline FunctionTranslator2<Parm1Type, Parm2Type, TypeOfReturn (*) ( TParm1Type, TParm2Type ) >
makeFunctor ( Functor2<Parm1Type,Parm2Type>* , TypeOfReturn (*f)( TParm1Type, TParm2Type ) )
{
	return FunctionTranslator2<Parm1Type, Parm2Type, TypeOfReturn (*)( TParm1Type, TParm2Type )>(f);
}

// "specialized" makeFunctor function for  pointer to non-const member function

template < class Parm1Type, class Parm2Type, class Callee,  class TypeOfReturn, class CallType, class TParm1Type, class TParm2Type  >
inline MemberTranslator2< Parm1Type, Parm2Type, Callee, TypeOfReturn (CallType::*) (TParm1Type, TParm2Type) >
makeFunctor ( Functor2<Parm1Type,Parm2Type>*, Callee &c, TypeOfReturn (CallType::*f) (TParm1Type,TParm2Type) )
{

	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type, TParm2Type );
	
	return MemberTranslator2< Parm1Type, Parm2Type, Callee, MemFunc > ( c, f );
}

// "specialized" makeFunctor function for pointers to const member function

template < class Parm1Type, class Parm2Type, class Callee, class TypeOfReturn, class CallType, class TParm1Type, class TParm2Type >
inline MemberTranslator2< Parm1Type, Parm2Type, const Callee, TypeOfReturn (CallType::*)( TParm1Type, TParm2Type ) const >
makeFunctor ( Functor2<Parm1Type,Parm2Type>* , const Callee &c, TypeOfReturn ( CallType::*f)( TParm1Type, TParm2Type ) const  )
{
	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type, TParm2Type ) const;
	
	return MemberTranslator2< Parm1Type, Parm2Type, const Callee, MemFunc>(c,f);
}


}

#endif // Functor2.hxx


