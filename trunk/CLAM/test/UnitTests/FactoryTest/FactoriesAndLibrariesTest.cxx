#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "Factory.hxx" // CLAM
#include "Processing.hxx" // CLAM

	

namespace CLAMTest
{


class FactoriesAndLibrariesTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactoriesAndLibrariesTest );

class FactoriesAndLibrariesTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoriesAndLibrariesTest );
	CPPUNIT_TEST( testCreate_LibraryRegisteredProduct );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() { }

	void tearDown() { }

private:


	void testCreate_LibraryRegisteredProduct()
	{

		typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
		
		ProcessingFactory& factory = ProcessingFactory::GetInstance();

		CLAM::Processing* product = factory.Create("SimpleOscillator");
	
		CPPUNIT_ASSERT_MESSAGE("Could not find SimpleOscillator on the library ProcessingFactory!",  product != NULL);
		
	}

};






} // namespace CLAMTest

