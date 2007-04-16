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

#ifndef __CBLMACROS__
#define __CBLMACROS__

#include "Functor0.hxx"
#include "Functor1.hxx"
#include "Functor2.hxx"
#include "Functor3.hxx"
#include "Functor4.hxx"

#include "Functor0R.hxx"
#include "Functor1R.hxx"
#include "Functor2R.hxx"
#include "Functor3R.hxx"
#include "Functor4R.hxx"

// Macros for wrapping Functor0

#define makeMemberFunctor0( ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor0*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor0( classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor0*)0, &classname::methodname ) \

#define makeCFunctionFunctor0( methodname ) \
	CBL::makeFunctor( (CBL::Functor0*)0, &methodname ) \

// Macros for wrapping Functor0R

#define makeMemberFunctor0R( returntype, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor0R<returntype>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor0R( returntype, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor0R<returntype>*)0, &classname::methodname ) \

#define makeCFunctionFunctor0R( returntype, methodname ) \
	CBL::makeFunctor( (CBL::Functor0R< returntype >*) 0, &methodname ) \

// Macros for wrapping Functor1

#define makeMemberFunctor1( parmtype, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor1< parmtype>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor1( parmtype, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor1< parmtype>*)0, &classname::methodname ) \

#define makeCFunctionFunctor1( parmtype, methodname ) \
	CBL::makeFunctor( (CBL::Functor1< parmtype>*)0, &methodname ) \

// Macros for wrapping Functor1R

#define makeMemberFunctor1R( returntype, parmtype, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor1R< returntype, parmtype>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor1R( returntype, parmtype, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor1R< returntype, parmtype>*)0, &classname::methodname ) \

#define makeCFunctionFunctor1R( returntype, parmtype, methodname ) \
	CBL::makeFunctor( (CBL::Functor1R< returntype, parmtype>*)0, &methodname ) \

// Macros for wrapping Functor2

#define makeMemberFunctor2( parmtype1, parmtype2, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor2< parmtype1, parmtype2>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor2( parmtype1, parmtype2, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor2< parmtype1, parmtype2>*)0, &classname::methodname ) \

#define makeCFunctionFunctor2( parmtype1, parmtype2, methodname ) \
	CBL::makeFunctor( (CBL::Functor2< parmtype1, parmtype2>*)0, &methodname ) \

// Macros for wrapping Functor2R

#define makeMemberFunctor2R( returntype, parmtype1, parmtype2, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor2R< returntype, parmtype1, parmtype2>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor2R( returntype, parmtype1, parmtype2, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor2R< returntype, parmtype1, parmtype2>*)0, &classname::methodname ) \

#define makeCFunctionFunctor2R( returntype, parmtype1, parmtype2, methodname ) \
	CBL::makeFunctor( (CBL::Functor2R< returntype, parmtype1, parmtype2>*)0, &methodname ) \


// Macros for wrapping Functor3

#define makeMemberFunctor3( parmtype1, parmtype2, parmtype3, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor3< parmtype1, parmtype2, parmtype3>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor3( parmtype1, parmtype2, parmtype3, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor3< parmtype1, parmtype2, parmtype3>*)0, &classname::methodname ) \

#define makeCFunctionFunctor3( parmtype1, parmtype2, parmtype3, methodname ) \
	CBL::makeFunctor( (CBL::Functor3< parmtype1, parmtype2, parmtype3>*)0, &methodname ) \

// Macros for wrapping Functor3R

#define makeMemberFunctor3R( returntype, parmtype1, parmtype2, parmtype3, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor3R< returntype, parmtype1, parmtype2, parmtype3>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor3R( returntype, parmtype1, parmtype2, parmtype3, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor3R< returntype, parmtype1, parmtype2, parmtype3>*)0, &classname::methodname ) \

#define makeCFunctionFunctor3R( returntype, parmtype1, parmtype2, parmtype3, methodname ) \
	CBL::makeFunctor( (CBL::Functor3R< returntype, parmtype1, parmtype2, parmtype3>*)0, &methodname ) \

// Macros for wrapping Functor4

#define makeMemberFunctor4( parmtype1, parmtype2, parmtype3, parmtype4, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor4< parmtype1, parmtype2, parmtype3, parmtype4>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor4( parmtype1, parmtype2, parmtype3, parmtype4, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor4< parmtype1, parmtype2, parmtype3, parmtype4>*)0, &classname::methodname ) \

#define makeCFunctionFunctor4( parmtype1, parmtype2, parmtype3, parmtype4, methodname ) \
	CBL::makeFunctor( (CBL::Functor4< parmtype1, parmtype2, parmtype3, parmtype4>*)0, &methodname ) \

// Macros for wrapping Functor4R

#define makeMemberFunctor4R( returntype, parmtype1, parmtype2, parmtype3, parmtype4, ref, classname, methodname ) \
	CBL::makeFunctor( (CBL::Functor4R< returntype, parmtype1, parmtype2, parmtype3, parmtype4>*)0, ref, &classname::methodname ) \

#define makeStaticMemberFunctor4R( returntype, parmtype1, parmtype2, parmtype3, parmtype4, classname, methodname) \
	CBL::makeFunctor( (CBL::Functor4R< returntype, parmtype1, parmtype2, parmtype3, parmtype4>*)0, &classname::methodname ) \

#define makeCFunctionFunctor4R( returntype, parmtype1, parmtype2, parmtype3, parmtype4, methodname ) \
	CBL::makeFunctor( (CBL::Functor4R< returntype, parmtype1, parmtype2, parmtype3, parmtype4>*)0, &methodname ) \


#endif // CBLMacros.hxx

