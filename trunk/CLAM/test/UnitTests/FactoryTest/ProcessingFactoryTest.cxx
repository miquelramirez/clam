#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "ProcessingFactory.hxx"


namespace CLAMTest
{


class ProcessingFactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingFactoryTest );

class ProcessingFactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ProcessingFactoryTest );
//	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();

protected:
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}

	// Tests definition :
protected:

	void test()
	{
		CPPUNIT_FAIL("RED");
	}
};



} // namespace CLAMTest

