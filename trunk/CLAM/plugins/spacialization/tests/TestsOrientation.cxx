#include "MiniCppUnit.hxx"
#include "AbsoluteCoordinates2RelativeAngles.hxx"


class TestsOrientation : public TestFixture<TestsOrientation>
{
public:
	typedef CLAM::Orientation Orientation;
	TEST_FIXTURE( TestsOrientation )
	{
/*		TEST_CASE( testDefaultInit );
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
		TEST_CASE( testToPoint_front );
		TEST_CASE( testToPoint_back );
		TEST_CASE( testToPoint_up );
		TEST_CASE( testToPoint_down );
		TEST_CASE( testToPoint_45left );
		TEST_CASE( testToPoint_45right );
		TEST_CASE( testToPoint_45up );
		TEST_CASE( testToPoint_45down );
		TEST_CASE( testToPoint_atOrigin );
		TEST_CASE( testRelativeOrientation_front );
		TEST_CASE( testRelativeOrientation_behind );
		TEST_CASE( testRelativeOrientation_arbitrary );
		TEST_CASE( testRelativeOrientation_arbitraryNegativeAzimuth );
		TEST_CASE( testChordDistance_back2back );
		TEST_CASE( testChordDistance_isSimmetric );
*/	}
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
	void testToPoint_front()
	{
		Orientation orientation;
		orientation.toPoint(1,0,0);
		ASSERT_EQUALS(Orientation(0,0), orientation);
	}
	void testToPoint_back()
	{
		Orientation orientation;
		orientation.toPoint(-1,0,0);
		ASSERT_EQUALS(Orientation(180,0), orientation);
	}
	void testToPoint_up()
	{
		Orientation orientation;
		orientation.toPoint(0,0,1);
		ASSERT_EQUALS(Orientation(0,90), orientation);
	}
	void testToPoint_down()
	{
		Orientation orientation;
		orientation.toPoint(0,0,-1);
		ASSERT_EQUALS(Orientation(0,-90), orientation);
	}
	void testToPoint_45left()
	{
		Orientation orientation;
		orientation.toPoint(1,1,0);
		ASSERT_EQUALS(Orientation(45,0), orientation);
	}
	void testToPoint_45right()
	{
		Orientation orientation;
		orientation.toPoint(1,-1,0);
		ASSERT_EQUALS(Orientation(315,0), orientation);
	}
	void testToPoint_45up()
	{
		Orientation orientation;
		orientation.toPoint(1,0,1);
		ASSERT_EQUALS(Orientation(0,45), orientation);
	}
	void testToPoint_45down()
	{
		Orientation orientation;
		orientation.toPoint(1,0,-1);
		ASSERT_EQUALS(Orientation(0,-45), orientation);
	}
	void testToPoint_atOrigin()
	{
		Orientation orientation;
		orientation.toPoint(0,0,0);
		ASSERT_EQUALS(Orientation(0,0), orientation);
	}


	// Tests Back2Back with the client code
	void testRelativeOrientation_front()
	{
		Orientation orientation = CLAM::AbsoluteCoordinates2RelativeAngles::
			computeRelativeOrientation
				( 0,0,0, 0,0,0, 1,0,0 );
		ASSERT_EQUALS(Orientation(0, 0), orientation);
	}	
	void testRelativeOrientation_behind()
	{
		Orientation orientation = CLAM::AbsoluteCoordinates2RelativeAngles::
			computeRelativeOrientation
				( 0,0,0, 0,0,0, -1,0,0 );
		ASSERT_EQUALS(Orientation(180, 0), orientation);
	}
	void testRelativeOrientation_arbitrary()
	{
		Orientation orientation = CLAM::AbsoluteCoordinates2RelativeAngles::
			computeRelativeOrientation
				( 0,0,0, 30,45,0, 1,1,1 );
		ASSERT_EQUALS(Orientation(12.340705747005989323, -8.5938519506549795324), orientation);
	}	
	void testRelativeOrientation_arbitraryNegativeAzimuth()
	{
		Orientation orientation = CLAM::AbsoluteCoordinates2RelativeAngles::
			computeRelativeOrientation
				( 0,0,0, 30,-45,0, 1,1,1 );
		ASSERT_EQUALS(Orientation(54.735610317245345868, 75.), orientation);
	}
	void testChordDistance_back2back()
	{
		Orientation orientation1(10,20);
		Orientation orientation2(30,-40);
		ASSERT_EQUALS(
			1.0868240888334650407,
			orientation1.chordDistance(orientation2));
	}
	void testChordDistance_isSimmetric()
	{
		Orientation orientation1(10,20);
		Orientation orientation2(30,-40);
		ASSERT_EQUALS(
			orientation1.chordDistance(orientation2),
			orientation2.chordDistance(orientation1));
	}
};

REGISTER_FIXTURE(TestsOrientation)


