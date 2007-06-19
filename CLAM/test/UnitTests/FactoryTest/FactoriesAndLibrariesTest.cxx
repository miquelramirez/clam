#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "ProcessingFactory.hxx"
#include "Processing.hxx"

	

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

		CLAM::ProcessingFactory& factory = CLAM::ProcessingFactory::GetInstance();
		CLAM::Processing* product = factory.Create("SimpleOscillator");
		CPPUNIT_ASSERT_MESSAGE("Could not find SimpleOscillator on the library ProcessingFactory!",  product != NULL);
		
	}

};






} // namespace CLAMTest

