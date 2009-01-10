#include <CLAM/RangeView.hxx>
#include <cppunit/extensions/HelperMacros.h>

namespace CLAMTest {
	
	class RangeViewTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( RangeViewTest );

	class RangeViewTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (RangeViewTest);

		CPPUNIT_TEST (testZoom_OutAtTheMiddle);
		CPPUNIT_TEST (testZoom_InAtTheMiddle);
		CPPUNIT_TEST (testZoom_InAtTheBegining);
		CPPUNIT_TEST (testZoom_OutAtTheBegining);
		CPPUNIT_TEST (testZoom_InAtTheEnd);
		CPPUNIT_TEST (testZoom_OutAtTheEnd);
		CPPUNIT_TEST (testKeepInRange_whenOverHighest);
		CPPUNIT_TEST (testKeepInRange_whenUnderLowest);
		CPPUNIT_TEST (testKeepInRange_whenDoesNotFit);

		CPPUNIT_TEST_SUITE_END();
	private:
		void testZoom_OutAtTheMiddle()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, 2, .5);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(13.2, high, 1e-14);
		}
		void testZoom_InAtTheMiddle()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, .5, .5);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.7, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(8.7, high, 1e-14);
		}
		void testZoom_InAtTheBegining()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, .5, 0.);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.2, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.2, high, 1e-14);
		}
		void testZoom_OutAtTheBegining()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, 2., 0.);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.2, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(16.2, high, 1e-14);
		}
		void testZoom_InAtTheEnd()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, .5, 1.);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(7.2, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(10.2, high, 1e-14);
		}
		void testZoom_OutAtTheEnd()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::zoom(low, high, 2., 1.);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.8, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(10.2, high, 1e-14);
		}

		void testKeepInRange_whenOverHighest()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::keepWithinInterval(low, high, 0, 10);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, high, 1e-14);
		}
		void testKeepInRange_whenUnderLowest()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::keepWithinInterval(low, high, 5, 20);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, high, 1e-14);
		}
		void testKeepInRange_whenDoesNotFit()
		{
			double low = 4.2;
			double high = 10.2;
			CLAM::RangeView::keepWithinInterval(low, high, 0, 3);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, low, 1e-14);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, high, 1e-14);
		}
	};

}




