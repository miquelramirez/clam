#include <cppunit/extensions/HelperMacros.h>



namespace Test {
	
	class RulerTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( RulerTest );

	class RulerTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (Test::RulerTest);
		CPPUNIT_TEST (test);
		CPPUNIT_TEST_SUITE_END();
	private:
		void test()
		{
			CPPUNIT_ASSERT_EQUAL(2.0, 2.0);
		}
	};


}




