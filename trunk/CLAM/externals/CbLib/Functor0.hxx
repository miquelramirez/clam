#ifndef __FUNCTOR0__
#define __FUNCTOR0__

#include "FunctorBase.hxx"

namespace CBL
{

class Functor0 : public FunctorBase
{
public:

	Functor0( DummyInit * = 0 )
	{
	}
	
	void operator()() const
	{
		thunk( *this );
	}
	
	FunctorBase::operator bool;
	
protected:
	
	typedef void (*Thunk) (const FunctorBase&);
	
	Functor0( Thunk t, const void* c, const void* f, _CSTD::size_t sz )
		: FunctorBase( c, f, sz ), thunk( t )
	{
	}
	
private:
	
	Thunk thunk;
	
};

	// Translator fors void f(void) signature callbacks
	
// Non-member or static function

template < class Func >
class FunctionTranslator0 : public Functor0
{
public:

	FunctionTranslator0 ( Func f )
		: Functor0( thunk, NULL, (void*)f, _CSTD::size_t(0) )
	{
	}
	
	static void thunk( const FunctorBase& ftor )
	{
		(Func(ftor.func))();
	}
};

// Member function

template < class Callee, class MemFunc >
class MemberTranslator0 : public Functor0
{
public:
	
	MemberTranslator0( Callee& c, const MemFunc& m )
		: Functor0 ( thunk, &c, &m, sizeof( MemFunc ) )
	{
	}
	
	static void thunk( const FunctorBase& ftor )
	{
		Callee* callee = (Callee*) ftor.callee;
#ifdef __MSVC__
		
		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(
		
		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
		
		(callee->*mfref)();
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		
		(callee->*memFunc)( );
#endif
	}
};

// makeFunctors helper functions

// "specialized" makeFunctor function for pointer to non-member or static function

template < class TypeOfReturn >
inline FunctionTranslator0< TypeOfReturn (*) () >
makeFunctor ( Functor0*, TypeOfReturn (*f) () )
{
	return FunctionTranslator0< TypeOfReturn (*) () >( f );
}

// "specialized" makeFunctor function for pointer to member function

template < class Callee, class TypeOfReturn, class CallType >
inline MemberTranslator0< Callee, TypeOfReturn (CallType::*) () >
makeFunctor ( Functor0*, Callee &c, TypeOfReturn (CallType::*f) () )
{

	typedef TypeOfReturn ( CallType::*MemFunc ) ();
	
	return MemberTranslator0< Callee, MemFunc > (c, f );
}

// "specialized" makeFunctor function for pointer to const member function

template < class Callee, class TypeOfReturn, class CallType >
inline MemberTranslator0< const Callee, TypeOfReturn (CallType::*) () const >
makeFunctor ( Functor0*, const Callee &c, TypeOfReturn (CallType::*f) () const )
{
	typedef TypeOfReturn ( CallType::*MemFunc ) () const;
	
	return MemberTranslator0< const Callee, MemFunc > (c , f );
}

}

#endif

