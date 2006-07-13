#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cppUnitHelper.hxx"

int main(void){
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test* theTest = registry.makeTest();
	CLAMTest::Helper::printTestNames( theTest );

	// We could just run() the suite. But using TestRunner we get
	// the exit code as well.
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	return !runner.run("");

}

