#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
//#include <cppunit/TextTestResult.h>

#include "cppUnitHelper.hxx"

int main(void){

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test* theTest = registry.makeTest();
	CLAMTest::Helper::printTestNames( theTest );
/*
	// The first way of running tests: TestSuite::run
	CppUnit::TestSuite suite;
	suite.addTest( theTest ); // TestSuite deletes its children
	CppUnit::TextTestResult res;
	suite.run(&res);
	std::cout << res << std::endl << std::endl;
*/

	// The other way of running a test suite: using TestRunner.
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	return !runner.run("");

}

