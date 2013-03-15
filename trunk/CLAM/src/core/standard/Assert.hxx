/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _ASSERT_
#define _ASSERT_

/** @file Assert.hxx
* Bug hunting and detection facilities for developers.
*/

#include "Err.hxx"

namespace CLAM
{

/** 
* @def CLAM_BREAKPOINT
* A macro containing break point assembler code for your platform.
* If you platform is not being considered by the header you will 
* get an warning message when compiling.
*/

// Microsoft VisualC++ and ancestror MSC
#ifdef _MSC_VER
#define CLAM_BREAKPOINT {__asm {int 3}}
	
// MetroWorks Code Warrior
#elif defined (__MWERKS__)
#define CLAM_BREAKPOINT {_asm {int 3}}

// GNU GCC
#elif defined (__GNUC__) && (defined  (__i386__) || defined(__x86_64__))
#define CLAM_BREAKPOINT {__asm__ (" int $3 "); }

/* g++ on powerpc linux */
#elif defined (__GNUC__) && defined  (__powerpc__)
#define CLAM_BREAKPOINT {__asm__ (" .long 0x7d821008 "); }

/* g++ on powerpc macosx */ 
#elif defined (__GNUC__) && defined  (__POWERPC__)
#define CLAM_BREAKPOINT {__asm__ (" .long 0x7d821008 "); }

// Insert your compiler here
#else
#warning Breakpoint code unknown for the platform. You can add it defining the CLAM_BREAKPOINT macro at file Assert.hxx.
#define CLAM_BREAKPOINT {}
#endif


#if ! defined(_DEBUG)
#define CLAM_USE_RELEASE_ASSERTS
#endif


/// Macro used when an assert fails
#if defined(CLAM_USE_RELEASE_ASSERTS)
#define CLAM_ABORT(message) \
	do { \
		throw CLAM::ErrAssertionFailed( message, __FILE__, __LINE__); \
	} while(0)
#else
#define CLAM_ABORT(message) \
	do { \
		if ( !CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled ) { \
			throw CLAM::ErrAssertionFailed( message, __FILE__, __LINE__); \
		} else { \
			CLAM::ExecuteAssertFailedHandler ( message, __FILE__, __LINE__); \
			CLAM_BREAKPOINT; \
		} \
	} while(0)
#endif

/// @name Assertions and checks
/// @{
/**
* @def CLAM_ASSERT(expresion,message)
* An assertion is a check on an expression that must be
* true given that all the contracts are fullfilled.
* Notice that this is NOT a regular (documented) check
* that will throw a (documented) error condition (Err).
* Asserts on debug mode (when the DEBUG macro is defined)
* will set a break point while in release mode (DEBUG is
* not defined) you may get an unexpected exception that
* can be catched by the application on the top level and
* do some backups actions.
* Any way, a descriptive message is displayed.
*
* Sometimes the check is not only an expression but a
* complete set of statements. In these cases, you can use
* CLAM_BEGIN_CHECK and CLAM_END_CHECK macros to enclose
* the whole check code and use CLAM_ASSERT for concrete
* expressions to check.
*
* You can change the default release mode assertion
* failed callback by using the SetAssertFailedHandler
* function.
*
* You can get a lightweight assertion using (with care)
* the CLAM_DEBUG_ASSERT that is ignored on debug mode.
* <b>By using the CLAM_DEBUG_ASSERT macro, the final
* application will not have the possibility of doing
* an honrous exit (backup data, bug log...)</b>
* Is worth to keep most of the asserts as CLAM_ASSERT.
*
* Anyway, for very performance depending applications
* you may use the CLAM_DISABLE_CHECKS to remove ALL the
* CLAM_ASSERT and CLAM_DEBUG_ASSERT and its related checks
* in whatever mode you use them.
*
* @param expression The expression that must be true.
* @param message A message that describes the unexpected
* runtime error to the programmer.
*/
/**
* @def CLAM_BEGIN_CHECK
* Marks the start of check code block that is tied to a CLAM_ASSERT
* and can be removed without affecting any functionality.
* You must use it on the same sense that CLAM_ASSERT is used.
* @see CLAM_ASSERT
* @see CLAM_END_CHECK
*/
/**
* @def CLAM_END_CHECK
* Ends a block of code started by CLAM_BEGIN_CHECK
* @see CLAM_BEGIN_CHECK
*/
#if defined(CLAM_DISABLE_CHECKS)
#define CLAM_BEGIN_CHECK if (0) {
#define CLAM_END_CHECK }
#define CLAM_ASSERT( expression, message )
#define CLAM_WARNING( expression, message )
#else
#define CLAM_BEGIN_CHECK {
#define CLAM_END_CHECK }
#define CLAM_ASSERT( expression, message ) \
	do { \
	if (!(expression)) { \
		CLAM_ABORT(message); \
	} } while (0)
#define CLAM_WARNING( expression, message ) \
	do { \
	if (!(expression)) { \
		CLAM::ExecuteWarningHandler ( message, __FILE__, __LINE__); \
	} } while (0)
#endif


/** 
* @def CLAM_BEGIN_DEBUG_CHECK
* Is the same that CLAM_BEGIN_CHECK but for a CLAM_DEBUG_ASSERT
* instead of a CLAM_ASSERT.
* @see CLAM_DEBUG_ASSERT
* @see CLAM_BEGIN_CHECK
* @see CLAM_END_DEBUG_CHECK
*/
/**
* @def CLAM_END_DEBUG_CHECK
* Ends a block of code started by CLAM_BEGIN_DEBUG_CHECK
* @see CLAM_BEGIN_DEBUG_CHECK
*/

#if defined(CLAM_DISABLE_CHECKS) || defined(CLAM_USE_RELEASE_ASSERTS)
#	define CLAM_BEGIN_DEBUG_CHECK if (0) {
#	define CLAM_END_DEBUG_CHECK }
#	define CLAM_DEBUG_ASSERT( expression, message )
#	define CLAM_DEBUG_WARNING( expression, message )
#else
#	define CLAM_BEGIN_DEBUG_CHECK {
#	define CLAM_END_DEBUG_CHECK }
#	define CLAM_DEBUG_ASSERT( expression, message ) \
		do { \
		if (!(expression)) { \
			CLAM_ABORT(message); \
		} } while (0)
#	define CLAM_DEBUG_WARNING( expression, message ) \
		do { \
		if (!(expression)) { \
			CLAM::ExecuteWarningHandler ( message, __FILE__, __LINE__); \
		} } while (0)
#endif
/// @}

/**
* The exception thrown when an assertion fails.
* Don't throw it directly, use the assertion macro Assert instead
* because the Assert macro is sensible to the compilation conditions.
* Neither do any explicit declaration that this exception can
* be thrown from a function as you would have to do with any other
* exception.
* See the Error Notification Mechanisms documentation.
* @todo Subclass ErrAssertionFailed from Err.
* @see CLAM_ASSERT
* @see CLAM_BEGIN_CHECK
* @see CLAM_END_CHECK
*/

class ErrAssertionFailed : public Err {
public:
	/** this bool is used for automatic-tesing CLAM asserts.
	 * by default is defined to true. But can be set to false where we
	 * want to test that a CLAM_ASSERT has occurred.
	 */
	static bool breakpointInCLAMAssertEnabled;

	ErrAssertionFailed(const char* message, const char* filename, int linenumber);
	virtual ~ErrAssertionFailed() throw () { }
};

/**
* The type of the asserts handlers.
* @param message A char pointer containing a description of the assertion
* @param fileName A char pointer containing the source file where the assertion is placed
* @param lineNumber The line of the source file where the assertion is placed
*/
typedef void (*AssertFailedHandlerType) (const char* message, const char* filename, int lineNumber);
/**
* Change the handler function called when an assertion fails.
* The function must have a prototype like that:
* <pre>
* void MyHandler (const char* message, const char* filename, int lineNumber);
* </pre>
* @param handler The new handler
* @return The old handler, for restoring purposes.
*/
AssertFailedHandlerType SetAssertFailedHandler(AssertFailedHandlerType handler);

/**
* (Don't use directly, use the CLAM_ASSERT macro instead) Execute the assert failed handler.
*/
void ExecuteAssertFailedHandler(const char* message, const char* filename, int linenumber);

/**
* The type of the warning handlers.
* @param message A char pointer containing a description of the warning
* @param fileName A char pointer containing the source file where the warning is placed
* @param lineNumber The line of the source file where the warning is placed
*/
typedef void (*WarningHandlerType) (const char* message, const char* filename, int lineNumber);
/**
* Change the handler function called when a warning is given.
* The function must have a prototype like that:
* <pre>
* void MyHandler (const char* message, const char* filename, int lineNumber);
* </pre>
* @param handler The new handler
* @return The old handler, for restoring purposes.
*/
WarningHandlerType SetWarningHandler(WarningHandlerType handler);

/**
* (Don't use directly, use the CLAM_WARNING macro instead) Execute the assert failed handler.
*/
void ExecuteWarningHandler(const char* message, const char* filename, int linenumber);


}

#endif //_ASSERT_

