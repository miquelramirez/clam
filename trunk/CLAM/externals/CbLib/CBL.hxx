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




