/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cppUnitHelper.hxx"
#include "Assert.hxx"

int main(void){
	// this flag allows clam asserts to behave throwing an exception instead
	// of doing breakpoint. Thus making auto-testing of asserts possible.
	// Notice: while debugging tests (if breakpoints are wanted back) you might need to
	// modify this flag before the point to be debugged.
	CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = false;

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test* theTest = registry.makeTest();
	CLAMTest::Helper::printTestNames( theTest );

	// We could just run() the suite. But using TestRunner we get
	// the exit code as well.
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	return !runner.run("");

}

