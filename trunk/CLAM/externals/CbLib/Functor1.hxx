#ifndef __FUNCTOR1__
#define __FUNCTOR1__

#include "FunctorBase.hxx"

namespace CBL
{

/**
 *	Note that we are inheriting protected from FunctorBase
 *  so we don't feel so ashamed because of the previous
 *  void*'s and unions
 */

template < class Parm1Type >
class Functor1 : public FunctorBase
{
public:
	
	Functor1( DummyInit * = 0 )
	{
	}
	
	void operator()( Parm1Type param1 ) const
	{
		thunk( *this, param1 );
	}
	
	FunctorBase::operator bool;
	
protected:

	typedef void (*Thunk) (const FunctorBase&, Parm1Type );
	
	Functor1( Thunk t, const void* c, const void* f, size_t sz )
		: FunctorBase( c, f, sz ), thunk( t )
	{
	}
	
private:

	Thunk thunk;
};
	
// Translators

// From non-member function

template < class Parm1Type, class Func > 
class FunctionTranslator1 : public Functor1< Parm1Type >
{
public:

	FunctionTranslator1( Func f ) 
		: Functor1< Parm1Type > ( thunk, NULL, (void*)f, size_t(0) )
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1 )
	{
		(Func(ftor.func))( param1 );
	}
};

// From member function

template < class Parm1Type, class Callee, class MemFunc >
class MemberTranslator1 : public Functor1< Parm1Type >
{
public:
	
	MemberTranslator1( Callee &c, const MemFunc& m )
		: Functor1< Parm1Type > (thunk, &c, &m, sizeof( MemFunc ) )
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1 )
	{
		Callee* callee = (Callee*) ftor.callee;
#ifdef __MSVC__

		// Micro$oft Parser is BIG TIME shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		(callee->*mfref)( param1 );
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		(callee->*memFunc)( param1 );
#endif
	}
};
	
// Traitized signature, so we can get rid of passing that NULL pointer and get
// overloading
// NOTE: It WONT work yet

template < class TypeOfReturn, class CallType, class TParm1Type >
struct Func1Signature
{
	typedef TypeOfReturn ( *Func ) ( TParm1Type );
	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type );
	typedef TypeOfReturn ( CallType::*ct_MemFunc ) ( TParm1Type );

};
	
// makeFunctors helper functions

// NOTE: There must be a mean available for the compiler to do an implicit cast between
// Parm1Type and TParm1Type

// "specialized" makeFunctor function for pointer to non-member functions



template < class Parm1Type, class TypeOfReturn, class TParm1Type >
inline FunctionTranslator1<Parm1Type, TypeOfReturn (*) ( TParm1Type ) >
makeFunctor ( Functor1<Parm1Type>* , TypeOfReturn (*f)( TParm1Type ) )
{
	return FunctionTranslator1<Parm1Type, TypeOfReturn (*)( TParm1Type )>(f);
}

// "specialized" makeFunctor function for  pointer to non-const member function



template < class Parm1Type, class Callee,  class TypeOfReturn, class CallType, class TParm1Type  >
inline MemberTranslator1< Parm1Type, Callee, TypeOfReturn (CallType::*) (TParm1Type) >
makeFunctor ( Functor1<Parm1Type>*, Callee &c, TypeOfReturn (CallType::* f) (TParm1Type) )
{

	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type );
	
	return MemberTranslator1< Parm1Type, Callee,MemFunc > ( c, f );

}

// "specialized" makeFunctor function for pointers to const member function

template < class Parm1Type, class Callee, class TypeOfReturn, class CallType, class TParm1Type >
inline MemberTranslator1< Parm1Type, const Callee, TypeOfReturn (CallType::*)( TParm1Type ) const >
makeFunctor ( Functor1<Parm1Type>* , const Callee &c, TypeOfReturn ( CallType::*f)( TParm1Type ) const  )
{
	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type ) const;
	
	return MemberTranslator1< Parm1Type, const Callee, MemFunc>(c,f);
}

}

#endif // Functor1.hxx
