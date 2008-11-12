#include "MiniCppUnit.hxx"
#include "AbsoluteCoordinates2RelativeAngles.hxx"


class TestsOrientation : public TestFixture<TestsOrientation>
{
public:
	typedef CLAM::Orientation Orientation;
	TEST_FIXTURE( TestsOrientation )
	{
		TEST_CASE( testDefaultInit );
		TEST_CASE( testChangingValues );
		TEST_CASE( testValuedInit );
		TEST_CASE( testNormalizeAngle_withZeroAngles );
		TEST_CASE( testNormalizeAngle_inPositiveQuadrant );
		TEST_CASE( testNormalizeAngle_withNegativeAzimut );
		TEST_CASE( testNormalizeAngle_withAzimutBeyond360 );
		TEST_CASE( testNormalizeAngle_withElevationBeyond90 );
		TEST_CASE( testNormalizeAngle_withElevationBeyondMinus90 );
		TEST_CASE( testNormalizeAngle_afterAdjustingElevationAzimuthStillInRange );
		TEST_CASE( testNormalizeAngle_withVeryNegativeAzimuth );
	}
	void testDefaultInit()
	{
		Orientation orientation;
		ASSERT_EQUALS("0 0", orientation );
	}
	void testChangingValues()
	{
		Orientation orientation;
		orientation.azimuth = 3.0;
		orientation.elevation = 4.0;
		ASSERT_EQUALS("3 4", orientation );
	}
	void testValuedInit()
	{
		Orientation orientation(3.,4.);
		ASSERT_EQUALS("3 4", orientation );
	}
	void testNormalizeAngle_withZeroAngles()
	{
		Orientation orientation(0.0, 0.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(0.0,0.0), orientation);
	}
	void testNormalizeAngle_inPositiveQuadrant()
	{
		Orientation orientation(10.0, 20.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(10.0,20.0), orientation);
	}
	void testNormalizeAngle_withNegativeAzimut()
	{
		Orientation orientation(-10.0, 20.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(350.0,20.0), orientation);
	}
	void testNormalizeAngle_withAzimutBeyond360()
	{
		Orientation orientation(370.0, 20.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(10.0,20.0), orientation);
	}
	void testNormalizeAngle_withElevationBeyond90()
	{
		Orientation orientation(10.0, 100.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(190.0,80.0), orientation);
	}
	void testNormalizeAngle_withElevationBeyondMinus90()
	{
		Orientation orientation(10.0, -100.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(190.0,-80.0), orientation);
	}
	void testNormalizeAngle_afterAdjustingElevationAzimuthStillInRange()
	{
		Orientation orientation(180.0, 100.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(0.0, 80.0), orientation);
	}
	void testNormalizeAngle_withVeryNegativeAzimuth()
	{
		Orientation orientation(-730.0, 20.0);
		orientation.normalize();
		ASSERT_EQUALS(Orientation(350.0,20.0), orientation);
	}
};

REGISTER_FIXTURE(TestsOrientation)


