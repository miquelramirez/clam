#ifndef __FUNCTOR3__
#define __FUNCTOR3__

#include "FunctorBase.hxx"

namespace CBL
{

template < class Parm1Type, class Parm2Type, class Parm3Type >
class Functor3 : public FunctorBase
{
public:
	
	Functor3( DummyInit * = 0 )
	{
	}
	
	void operator()( Parm1Type param1, Parm2Type param2, Parm3Type param3 ) const
	{
		thunk( *this, param1, param2, param3 );
	}
	
	FunctorBase::operator bool;
	
protected:

	typedef void (*Thunk) (const FunctorBase&, Parm1Type, Parm2Type, Parm3Type );
	
	Functor3( Thunk t, const void* c, const void* f, _CSTD::size_t sz )
		: FunctorBase( c, f, sz ), thunk( t )
	{
	}
	
private:

	Thunk thunk;
};
	
// Translators

// From non-member function

template < class Parm1Type, class Parm2Type, class Parm3Type, class Func > 
class FunctionTranslator3 : public Functor3< Parm1Type, Parm2Type, Parm3Type >
{
public:

	FunctionTranslator3( Func f ) 
		: Functor3< Parm1Type, Parm2Type, Parm3Type > ( thunk, NULL, (void*)f, _CSTD::size_t(0) )
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3 )
	{
		(Func(ftor.func))( param1, param2, param3 );
	}
};

// From member function

template < class Parm1Type, class Parm2Type, class Parm3Type, class Callee, class MemFunc >
class MemberTranslator3 : public Functor3< Parm1Type, Parm2Type, Parm3Type >
{
public:
	
	MemberTranslator3( Callee &c, const MemFunc& m )
		: Functor3< Parm1Type, Parm2Type, Parm3Type > (thunk, &c, &m, sizeof( MemFunc ) )
	{
	}
	
	static void thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2, Parm3Type param3 )
	{
		Callee* callee = (Callee*) ftor.callee;
#ifdef __MSVC__

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		(callee->*mfref)(param1,param2,param3);
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		(callee->*memFunc)( param1, param2, param3 );
#endif
	}
};

// makeFunctors helper functions

// NOTE: There must be a mean available for the compiler to do an implicit cast between
// Parm1Type and TParm1Type

// "specialized" makeFunctor function for pointer to non-member functions

template < class Parm1Type, class Parm2Type, class Parm3Type, class TypeOfReturn, class TParm1Type, class TParm2Type, class TParm3Type >
inline FunctionTranslator3<Parm1Type, Parm2Type, Parm3Type, TypeOfReturn (*) ( TParm1Type, TParm2Type, TParm3Type ) >
makeFunctor ( Functor3<Parm1Type,Parm2Type,Parm3Type>* , TypeOfReturn (*f)( TParm1Type, TParm2Type, TParm3Type ) )
{
	return FunctionTranslator3<Parm1Type, Parm2Type, Parm3Type, TypeOfReturn (*)( TParm1Type, TParm2Type, TParm3Type )>(f);
}

// "specialized" makeFunctor function for  pointer to non-const member function

template < class Parm1Type, class Parm2Type, class Parm3Type, class Callee,  class TypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type  >
inline MemberTranslator3< Parm1Type, Parm2Type, Parm3Type, Callee, TypeOfReturn (CallType::*) (TParm1Type, TParm2Type, TParm3Type) >
makeFunctor ( Functor3<Parm1Type,Parm2Type,Parm3Type>*, Callee &c, TypeOfReturn (CallType::*f) (TParm1Type,TParm2Type,TParm3Type) )
{

	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type, TParm2Type, TParm3Type );
	
	return MemberTranslator3< Parm1Type, Parm2Type, Parm3Type, Callee, MemFunc > ( c, f );
}

// "specialized" makeFunctor function for pointers to const member function

template < class Parm1Type, class Parm2Type, class Parm3Type, class Callee, class TypeOfReturn, class CallType, class TParm1Type, class TParm2Type, class TParm3Type >
inline MemberTranslator3< Parm1Type, Parm2Type, Parm3Type, const Callee, TypeOfReturn (CallType::*)( TParm1Type, TParm2Type, TParm3Type ) const >
makeFunctor ( Functor3<Parm1Type,Parm2Type,Parm3Type>* , const Callee &c, TypeOfReturn ( CallType::*f)( TParm1Type, TParm2Type, TParm3Type ) const  )
{
	typedef TypeOfReturn ( CallType::*MemFunc ) ( TParm1Type, TParm2Type, TParm3Type ) const;
	
	return MemberTranslator3< Parm1Type, Parm2Type, Parm3Type, const Callee, MemFunc>(c,f);
}



}

#endif // Functor3.hxx


