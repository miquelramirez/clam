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

#include "Assert.hxx"
#include <iostream>
#if defined(__linux__)
#include <execinfo.h>
#endif


namespace CLAM {

void DumpBacktrace(std::ostream & os)
{
#if defined(__linux__)

	void *bt_array[100];	// 100 should be enough ?!?
	char **bt_strings;
	int num_entries;

	if ((num_entries = backtrace(bt_array, 100)) < 0) {
		os << " Unable to generate a backtrace" << std::endl;
		return;
	}

	if ((bt_strings = backtrace_symbols(bt_array, num_entries)) == NULL) {
		os << " Unable to adquire symbols names for the backtrace" << std::endl;
		return;
	}

	os << "\n Backtrace:\n" << std::endl;
	for (int i = 0; i < num_entries; i++) {
		os << "[" << i << "] " <<  bt_strings[i] << std::endl;
	}
	free(bt_strings);
#else
	os << " Unable to adquire symbols names for the backtrace" << std::endl;
#endif
}


// by default, CLAM asserts must breakpoint
// we'll want to disable breakpoints for automatic assertion testing 
// purposes
bool disabledCLAMAssertBreakpoint = false;

// Assert related

static void DefaultAssertHandler(const char* message, const char* filename, int lineNumber )
{
	std::cout << "##########################################################" << std::endl;
	std::cout << "################### ASSERTION FAILED #####################" << std::endl;
	std::cout << "##########################################################" << std::endl;
	std::cout << "At file " << filename << " line " << lineNumber << std::endl;
	std::cout << message << std::endl;
	DumpBacktrace(std::cout);
}

static AssertFailedHandlerType CurrentAssertFailedHandler=DefaultAssertHandler;

AssertFailedHandlerType SetAssertFailedHandler(AssertFailedHandlerType handler) {
	AssertFailedHandlerType oldHandler = CurrentAssertFailedHandler;
	CurrentAssertFailedHandler = handler;
	return oldHandler;
}

void ExecuteAssertFailedHandler(const char* message, const char* filename, int lineNumber )
{
	CurrentAssertFailedHandler(message,filename,lineNumber);
}

bool ErrAssertionFailed::breakpointInCLAMAssertEnabled = true;

ErrAssertionFailed::ErrAssertionFailed(const char* message, const char* filename, int lineNumber)
	: Err(message)
{
	if (!breakpointInCLAMAssertEnabled) return; 

	CurrentAssertFailedHandler( message, filename, lineNumber );
}

// Warning related

static void DefaultWarningHandler(const char* message, const char* filename, int lineNumber )
{
	std::cout << "##########################################################" << std::endl;
	std::cout << "######################## WARNING #########################" << std::endl;
	std::cout << "##########################################################" << std::endl;
	std::cout << "At file " << filename << " line " << lineNumber << std::endl;
	std::cout << message << std::endl;
}

static WarningHandlerType CurrentWarningHandler=DefaultWarningHandler;

WarningHandlerType SetWarningHandler(WarningHandlerType handler) {
	WarningHandlerType oldHandler = CurrentWarningHandler;
	CurrentWarningHandler = handler;
	return oldHandler;
}

void ExecuteWarningHandler(const char* message, const char* filename, int lineNumber )
{
	CurrentWarningHandler(message,filename,lineNumber);
}


}


