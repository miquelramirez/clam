#ifndef __CBL__
#define __CBL__

// Functors without return

#ifdef WIN32
#define __MSVC__
#endif

// void foo( void )-like signature
#include "Functor0.hxx"

// void foo( type1 )-like signature
#include "Functor1.hxx"

// void foo( type1, type2 )-like signature
#include "Functor2.hxx"

// void foo( type1, type2, type3 )-like signature
#include "Functor3.hxx"

// void foo( type1, type2, type3, type4 )-like signature
#include "Functor4.hxx"


// Functors with return

// type_r foo()-like signature
#include "Functor0R.hxx"

// type_r foo ( type1 )-like signature
#include "Functor1R.hxx"

// type_r foo ( type1, type2 )-like signature
#include "Functor2R.hxx"

// type_r foo ( type1, type2, type3 )-like signature
#include "Functor3R.hxx"

// type_r foo ( type1, type2, type3, type4 )-like signature
#include "Functor4R.hxx"

#include "CBLMacros.hxx"

#endif // __CBL__




