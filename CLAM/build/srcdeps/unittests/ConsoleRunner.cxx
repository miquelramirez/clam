#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>

// todo: get rid of mandatory global vars like this;
extern "C" 
{
#	include "../verbose.h"
	int verbose = 0;
}


	class Helper
	{
	private:
		static void indent(unsigned n){
			for (unsigned i=0; i<n; i++) std::cout << " ";
		}
		typedef std::vector<CppUnit::Test *> Tests;
		
	public:
		static void printTestNames(const CppUnit::Test* test, const unsigned ind=0)
		{
			
			const CppUnit::TestSuite * suite = dynamic_cast<const CppUnit::TestSuite*>( test );
			if (suite) {
				std::cout << std::endl;
				indent(ind);
				std::cout << "+ " << suite->getName() << std::endl;
				Tests::const_iterator it;
				for (it=suite->getTests().begin(); it!=suite->getTests().end(); it++ )
					printTestNames(*it, ind+4);
			} else {
				std::string testName = test->getName();
				indent(ind);
				std::cout << ". "
					<< testName.replace(0,testName.find(".")+1,"")
					<< std::endl;
			}
			if (ind==0) {
				std::cout << "\n\n";
			}
		} 
	};


int main(void){

	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::Test* theTest = registry.makeTest();
	Helper::printTestNames( theTest );

	// We could just run() the suite. But using TestRunner we get
	// the exit code as well.
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( theTest ); // caution: it deletes the suite on termination.
	return !runner.run("");

}

