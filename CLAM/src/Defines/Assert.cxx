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


namespace CLAM {

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



