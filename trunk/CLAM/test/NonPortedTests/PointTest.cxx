#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

namespace CLAMTest
{


class PointTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PointTest );

class PointTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PointTest );
	CPPUNIT_TEST( testSomeMethod_WhenSomeConditionsMeet );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testSomeMethod_WhenSomeConditionsMeet()
	{
		// Setup

		// Exercise

		// Verification
		CPPUNIT_ASSERT_EQUAL( *** Expected Result ***, *** Given Result *** );
		
		// Tear down
	}
};






} // namespace CLAMTest

