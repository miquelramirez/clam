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

#include "Assert.hxx"
#include <iostream>
#include <cstdlib>
#include <csignal>
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
	std::cerr << "##########################################################" << std::endl;
	std::cerr << "################### ASSERTION FAILED #####################" << std::endl;
	std::cerr << "##########################################################" << std::endl;
	std::cerr << "At file " << filename << " line " << lineNumber << std::endl;
	std::cerr << message << std::endl;
	DumpBacktrace(std::cerr);
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
	std::cerr << "##########################################################" << std::endl;
	std::cerr << "######################## WARNING #########################" << std::endl;
	std::cerr << "##########################################################" << std::endl;
	std::cerr << "At file " << filename << " line " << lineNumber << std::endl;
	std::cerr << message << std::endl;
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

#if 0 //  defined(__linux__)
class SystemSignalTrapper
{
	int _signal;
	sighandler_t _oldHandler;
public:
	SystemSignalTrapper(int signal, sighandler_t handler) :
		_signal(signal)
	{
		_oldHandler = std::signal(signal, handler);
	}
	~SystemSignalTrapper()
	{
		std::signal(_signal, _oldHandler);
	}
};
void segvSignalHandler(int myInt)
{
	std::cerr << std::endl;
	std::cerr << "##########################################################" << std::endl;
	std::cerr << "#################### BAD MEMORY ACCES ####################" << std::endl;
	std::cerr << "##########################################################" << std::endl;
	DumpBacktrace(std::cerr);
	std::abort();
}

static SystemSignalTrapper segvSignalTrapper(SIGSEGV,segvSignalHandler);
#endif //defined linux


}


