#ifndef __FUNCTOR0R__
#define __FUNCTOR0R__

#include "FunctorBase.hxx"

namespace CBL
{

template < class TypeOfReturn >
class Functor0R : public FunctorBase
{
public:

	Functor0R( DummyInit * = 0 )
	{
	}
	
	TypeOfReturn operator()() const
	{
		return thunk( *this );
	}

	FunctorBase::operator bool;

protected:

	typedef TypeOfReturn (*Thunk) (const FunctorBase&);
	
	Functor0R( Thunk t, const void* c, const void* f, size_t sz )
		: FunctorBase( c, f, sz ), thunk( t )
	{
	}
	
private:

	Thunk thunk;

};

// Translator fors void f(void) signature callbacks

// Non-member or static function

template < class TypeOfReturn, class Func >
class FunctionTranslator0R : public Functor0R<TypeOfReturn>
{
public:

	FunctionTranslator0R ( Func f )
		: Functor0R<TypeOfReturn>( thunk, NULL, (void*)f, size_t(0) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor )
	{
		return (Func(ftor.func))();
	}
};

// Member function

template <class TypeOfReturn, class Callee, class MemFunc >
class MemberTranslator0R : public Functor0R<TypeOfReturn>
{
public:
	
	MemberTranslator0R( Callee& c, const MemFunc& m )
		: Functor0R<TypeOfReturn> ( thunk, &c, &m, sizeof( MemFunc ) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor )
	{
		Callee* callee = (Callee*) ftor.callee;
#ifdef _MSC_VER

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		return (callee->*mfref)( );
		
#else	
		
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		
		return (callee->*memFunc)( );
#endif
	}
};

// makeFunctors helper functions

// "specialized" makeFunctor function for pointer to non-member or static function

template < class TypeOfReturn, class TTypeOfReturn >
inline FunctionTranslator0R< TypeOfReturn, TTypeOfReturn (*) () >
makeFunctor ( Functor0R<TypeOfReturn>*, TTypeOfReturn (*f) () )
{
	return FunctionTranslator0R< TypeOfReturn, TTypeOfReturn (*) () >( f );
}

// "specialized" makeFunctor function for pointer to member function

template < class Callee, class TypeOfReturn, class CallType, class TTypeOfReturn >
inline MemberTranslator0R< TypeOfReturn, Callee, TTypeOfReturn (CallType::*) () >
makeFunctor ( Functor0R<TypeOfReturn>*, Callee &c, TTypeOfReturn (CallType::*f) () )
{

	typedef TTypeOfReturn ( CallType::*MemFunc ) ();
	
	return MemberTranslator0R< TypeOfReturn, Callee, MemFunc > (c, f );
}

// "specialized" makeFunctor function for pointer to const member function

template < class Callee, class TypeOfReturn, class CallType, class TTypeOfReturn >
inline MemberTranslator0R< TypeOfReturn, const Callee, TTypeOfReturn (CallType::*) () const >
makeFunctor ( Functor0R<TypeOfReturn>*, const Callee &c, TTypeOfReturn (CallType::*f) () const )
{
	typedef TTypeOfReturn ( CallType::*MemFunc ) () const;
	
	return MemberTranslator0R<TypeOfReturn, const Callee, MemFunc > (c , f );
}


}

#endif // Functor0R.hxx



