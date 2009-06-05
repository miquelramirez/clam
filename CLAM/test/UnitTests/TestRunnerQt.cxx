#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/qt/TestRunner.h>

#include <QtGui/QApplication>

#include "cppUnitHelper.hxx"
#include "Assert.hxx"

int main(int argc, char ** argv){
	// this flag allows clam asserts to behave throwing an exception instead
	// of doing breakpoint. Thus making auto-testing of asserts possible.
	// Notice: while debugging tests (if breakpoints are wanted back) you might need to
	// modify this flag before the point to be debugged.
	CLAM::ErrAssertionFailed::breakpointInCLAMAssertEnabled = false;

	QApplication application(argc, argv);

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test* theTest = registry.makeTest();
	CLAMTest::Helper::printTestNames( theTest );

	// The other way of running a test suite: using TestRunner.
	CppUnit::QtUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	runner.run("");
	return 0;
}

