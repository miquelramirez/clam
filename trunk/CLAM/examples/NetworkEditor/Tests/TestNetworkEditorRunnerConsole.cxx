
#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cppUnitHelper.hxx"
#include "Assert.hxx"
#include <qapplication.h>

int main(int argc, char ** argv)
{

	QApplication app( argc, argv );

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

