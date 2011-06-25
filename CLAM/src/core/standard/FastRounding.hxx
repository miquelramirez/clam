/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __FASTROUNDING__
#define __FASTROUNDING__

#include "Assert.hxx"
#include <cmath>
#ifdef WIN32
#include <float.h>
#endif

#if defined (_MSC_VER )
/**IMPORTANT: if in release mode, you are responsible for changing controlfp.
   You must do so outside the loop that actually calls the loop */


#if !defined (_DEBUG)

/**
   MRJ: How to use these macros

   They are quite straightforward to use. Whenever you have a loop like the one below:

   for ( int k = 0; ... )
   {
      stuff.DoSomething(); // DoSomething uses internally Round or Chop
   }

   you must protect the loop with these macros like:

   CLAM_ACTIVATE_FAST_ROUNDING;
   for ( int k = 0; ... )
   {
      stuff.DoSomething(); // DoSomething uses internally Round or Chop
   }
   CLAM_DEACTIVATE_FAST_ROUNDING;

   However note that both macros must be at the same *scope*, so things like:

   if ( something )
   {
       CLAM_ACTIVATE_FAST_ROUNDING;
       for( int k=0; ... )
       {
           stuff.DoSomething(); // DoSomething uses internally Round or Chop
       }
   }
   CLAM_DEACTIVATE_FAST_ROUNDING;

   won't compile. The compiler should complain about a variable called
   'CLAM_FPU_STATE_WORD' not being declared.
   The right way is to do:

   if ( something )
   {
       CLAM_ACTIVATE_FAST_ROUNDING;
       for( int k=0; ... )
       {
           stuff.DoSomething(); // DoSomething uses internally Round or Chop
       }
       CLAM_DEACTIVATE_FAST_ROUNDING;
   }
   
   Note that these macros are a temporary solution, suggestions welcome.

*/

#define CLAM_ACTIVATE_FAST_ROUNDING \
unsigned CLAM_FPU_STATE_WORD;\
CLAM_FPU_STATE_WORD = _controlfp( 0, 0 );\

#define CLAM_DEACTIVATE_FAST_ROUNDING \
_controlfp( CLAM_FPU_STATE_WORD, _MCW_RC ); \

//optimized positive integer chopping routine for Windows, is equivalent to int(a) but much more efficient
inline int Chop( float a )
{
	int i;
	CLAM_DEBUG_ASSERT( a>=0, "Chop function only for positive numbers" );
	/** MRJ:The implicit branch here negates the performance increase that this functions
	    is supposed to bring
	CLAM_ASSERT( (_controlfp(0, 0) & _MCW_RC) == _RC_CHOP, 
		     "Rounding mode of FPU control word must be set to CHOP for Chop().");
	*/
	_asm {
		fld a
		fistp i
	}

	return i;
}

inline int Round( float a )
{
	CLAM_DEBUG_ASSERT(a>=0,"Round function only for positive numbers");
	int i;
	static const float half = 0.5f;
	/** MRJ:The implicit branch here negates the performance increase that this functions
	    is supposed to bring
	CLAM_ASSERT((_controlfp(0, 0) & _MCW_RC) == _RC_CHOP, 
		    "Rounding mode of FPU control word must be set to CHOP for Chop().");
	*/
	__asm {
		fld   a
		fadd  half
		fistp i
        }

	return i;
	
}

#else // Debug mode

// MRJ: The macros do nothing on DEBUG
#define CLAM_ACTIVATE_FAST_ROUNDING
#define CLAM_DEACTIVATE_FAST_ROUNDING

inline int Chop( float a )
{
	int i;
	CLAM_DEBUG_ASSERT( a>=0, "Chop function only for positive numbers" );
	unsigned int saved = _controlfp(0, 0);
	_controlfp(_RC_CHOP, _MCW_RC);

	_asm {
		fld a
		fistp i
	}

	_controlfp(saved, _MCW_RC);	
	return i;
}

inline int Round( float a )
{
	CLAM_DEBUG_ASSERT(a>=0,"Round function only for positive numbers");
	int i;
	static const float half = 0.5f;

	unsigned int saved = _controlfp(0, 0);
	_controlfp(_RC_CHOP, _MCW_RC);
	
	__asm {
		fld   a
		fadd  half
		fistp i
        }
	
	_controlfp(saved, _MCW_RC);

	return i;
	
}

#endif // End of DEBUG check

#else // Not Microsoft Visual C

// The macros don't do anything in compilers other than MSVC
#define CLAM_ACTIVATE_FAST_ROUNDING
#define CLAM_DEACTIVATE_FAST_ROUNDING

inline int Chop( float a )
{
	return int(a);// just hope it's an intrinsic.
}

inline int Round( float a )
{
	#ifdef __USE_ISOC99
		return lrint(a);
	#else
		return int(rint(a));
	#endif	
}

#endif


#endif // FastRounding.hxx

