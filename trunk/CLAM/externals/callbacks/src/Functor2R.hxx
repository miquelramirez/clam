#ifndef __FUNCTOR2R__
#define __FUNCTOR2R__

#include "FunctorBase.hxx"

namespace CBL
{

template < class TypeOfReturn, class Parm1Type, class Parm2Type >
class Functor2R : public FunctorBase
{
public:

	Functor2R( DummyInit * = 0 )
	{
	}
	
	TypeOfReturn operator()( Parm1Type param1, Parm2Type param2 ) const
	{
		return thunk( *this, param1, param2 );
	}
	
	FunctorBase::operator bool;
	
protected:
	
	typedef TypeOfReturn (*Thunk) (const FunctorBase&, Parm1Type, Parm2Type );
	
	Functor2R( Thunk t, const void* c, const void* f, _CSTD::size_t sz )
		: FunctorBase( c, f, sz), thunk ( t )
	{
	}

private:

	Thunk thunk;

};

// Translators

// From non-member function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Func >
class FunctionTranslator2R : public Functor2R< TypeOfReturn, Parm1Type, Parm2Type >
{
public:

	FunctionTranslator2R( Func f )
		: Functor2R< TypeOfReturn, Parm1Type, Parm2Type >( thunk, NULL, (void*)f, _CSTD::size_t(0))
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2 )
	{
		return (Func( ftor.func )) ( param1, param2 );
	}

};

// From member Function

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Callee, class MemFunc >
class MemberTranslator2R : public Functor2R< TypeOfReturn, Parm1Type, Parm2Type >
{
public:

	MemberTranslator2R( Callee& c, const MemFunc& mf )
		: Functor2R< TypeOfReturn, Parm1Type, Parm2Type > ( thunk, &c, &mf, sizeof( MemFunc ) )
	{
	}
	
	static TypeOfReturn thunk( const FunctorBase& ftor, Parm1Type param1, Parm2Type param2 )
	{
	
		Callee* callee = (Callee*) ftor.callee;
#ifdef __MSVC__

		// Microsoft Parser is shit, just because it didn't like MemFunc &memFunc( *(MemFunc*)(void*)(ftor.memFunc) );
		// I HAD to make this horrible kludge :_(

		void* pvoid = (void*) ftor.memFunc;
		MemFunc* pfunc = (MemFunc*) pvoid;
		
		MemFunc& mfref = *pfunc;
	
		return (callee->*mfref)(param1,param2);
		
#else
		MemFunc &memFunc(*(MemFunc*)(void*)(ftor.memFunc));
		
		return (callee->*memFunc)( param1, param2 );
#endif
	}
};

// "specialized" makeFunctor function for pointers to non-member or static functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class TTypeOfReturn, class TParm1Type, class TParm2Type >
inline FunctionTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, TTypeOfReturn (*)(TParm1Type, TParm2Type ) >
makeFunctor ( Functor2R< TypeOfReturn, Parm1Type, Parm2Type >*, TTypeOfReturn (*f) (TParm1Type, TParm2Type) )
{
	return FunctionTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, TTypeOfReturn (*)(TParm1Type, TParm2Type) >( f );
}

// "specialized" makeFunctor function to pointer to non-const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type >
inline MemberTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type) >
makeFunctor ( Functor2R< TypeOfReturn, Parm1Type, Parm2Type >*, Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type) )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type);
	
	return MemberTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, Callee, MemFunc >( c, f );
}

// "specialized" makeFunctor function to pointer to const member functions

template < class TypeOfReturn, class Parm1Type, class Parm2Type, class Callee, class TTypeOfReturn, class CallType, class TParm1Type, class TParm2Type >
inline MemberTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, const Callee, TTypeOfReturn (CallType::*) (TParm1Type, TParm2Type) const >
makeFunctor ( Functor2R< TypeOfReturn, Parm1Type, Parm2Type >*, const Callee& c, TTypeOfReturn (CallType::*f) (TParm1Type, TParm2Type) const )
{

	typedef TTypeOfReturn (CallType::*MemFunc) (TParm1Type, TParm2Type) const;
	
	return MemberTranslator2R< TypeOfReturn, Parm1Type, Parm2Type, const Callee, MemFunc >( c, f );
}


}

#endif // Functor2R.hxx

