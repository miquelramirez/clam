#include <CLAM/RulerTicks.hxx>
#include <cppunit/extensions/HelperMacros.h>

/*
  TODO:
  - 


 */


namespace CLAMTest {
	
	class RulerTicksTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( RulerTicksTest );

	class RulerTicksTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (RulerTicksTest);

		CPPUNIT_TEST (testOffsetAndGap_withUnitGap);
		CPPUNIT_TEST (testOffsetAndGap_withHalfWidth);
		CPPUNIT_TEST (testOffsetAndGap_withMinGap2);
		CPPUNIT_TEST (testOffsetAndGap_withMinGap3);
		CPPUNIT_TEST (testOffsetAndGap_withOffsetAndMarkOnOffset);
		CPPUNIT_TEST (testOffsetAndGap_withOffsetAndMarkNotOnOffset);
		CPPUNIT_TEST (testOffsetAndGap_withNegativeRange);
		CPPUNIT_TEST (testOffsetAndGap_withZoomOut);

		CPPUNIT_TEST_SUITE_END();
	private:
		void testOffsetAndGap_withUnitGap()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(0,10);
			ruler.setMinGap(1);
			ruler.setWidth(10);
			CPPUNIT_ASSERT_EQUAL(1.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(0.0, ruler.markOffset());
		}
		void testOffsetAndGap_withHalfWidth()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(0,10);
			ruler.setMinGap(1);
			ruler.setWidth(5);
			CPPUNIT_ASSERT_EQUAL(2.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(0.0, ruler.markOffset());
		}
		void testOffsetAndGap_withMinGap2()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(0,10);
			ruler.setMinGap(2);
			ruler.setWidth(10);
			CPPUNIT_ASSERT_EQUAL(2.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(0.0, ruler.markOffset());
		}
		void testOffsetAndGap_withMinGap3()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(0,10);
			ruler.setMinGap(3);
			ruler.setWidth(10);
			CPPUNIT_ASSERT_EQUAL(5.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(0.0, ruler.markOffset());
		}
		void testOffsetAndGap_withOffsetAndMarkOnOffset()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(2,12);
			ruler.setMinGap(1);
			ruler.setWidth(10);
			CPPUNIT_ASSERT_EQUAL(1.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(2.0, ruler.markOffset());
		}
		void testOffsetAndGap_withNegativeRange()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(-10,10);
			ruler.setMinGap(10);
			ruler.setWidth(100);
			CPPUNIT_ASSERT_EQUAL(2.0, ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(-10.0, ruler.markOffset());
		}
		void testOffsetAndGap_withZoomOut()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(0,100);
			ruler.setMinGap(3);
			ruler.setWidth(10);
			CPPUNIT_ASSERT_EQUAL(50., ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(0.0, ruler.markOffset());
		}
		void testOffsetAndGap_withOffsetAndMarkNotOnOffset()
		{
			CLAM::RulerTicks ruler;
			ruler.setRange(4,104);
			ruler.setMinGap(10);
			ruler.setWidth(100);
			CPPUNIT_ASSERT_EQUAL(10., ruler.markGap());
			CPPUNIT_ASSERT_EQUAL(10.0, ruler.markOffset());
		}
	};


}




