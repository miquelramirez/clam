#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Stats.hxx"

namespace CLAMTest
{



class StatsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( StatsTest );

class StatsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( StatsTest );
	CPPUNIT_TEST( testCentroid );
	CPPUNIT_TEST( testMoment1 );
	CPPUNIT_TEST( testMoment2 );
	CPPUNIT_TEST( testMoment3 );
	CPPUNIT_TEST( testMoment4 );
	CPPUNIT_TEST( testMoment5 );
	CPPUNIT_TEST( testMoment6 );
	CPPUNIT_TEST( testSlope_withPrimesUp );
	CPPUNIT_TEST( testSlope_withPrimesDown );
	CPPUNIT_TEST( testSlope_withConstant );
	CPPUNIT_TEST( testSlope_withInvertedUp );
	CPPUNIT_TEST( testTilt_withPrimesUp );
	CPPUNIT_TEST( testTilt_withPrimesDown );
	CPPUNIT_TEST( testTilt_withConstant );
	CPPUNIT_TEST( testTilt_withCountUp );
	CPPUNIT_TEST( testTilt_withCountDown );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}
	
private:
	void fillPrimesUpInverted()
	{
		_array.AddElem(1/1.);
		_array.AddElem(1/2.);
		_array.AddElem(1/3.);
		_array.AddElem(1/5.);
	}
	void fillPrimesUp()
	{
		_array.AddElem(1.);
		_array.AddElem(2.);
		_array.AddElem(3.);
		_array.AddElem(5.);
	}
	void fillConstant()
	{
		_array.AddElem(3.);
		_array.AddElem(3.);
		_array.AddElem(3.);
		_array.AddElem(3.);
	}

	void fillPrimesDown()
	{
		_array.AddElem(5.);
		_array.AddElem(3.);
		_array.AddElem(2.);
		_array.AddElem(1.);
	}

	void fillCountUp()
	{
		_array.AddElem(1.);
		_array.AddElem(2.);
		_array.AddElem(3.);
		_array.AddElem(4.);
	}
	void fillCountDown()
	{
		_array.AddElem(4.);
		_array.AddElem(3.);
		_array.AddElem(2.);
		_array.AddElem(1.);
	}

	CLAM::Array<CLAM::TData> _array;

private:
	void testCentroid()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(2.09091), stats.GetCentroid(), 0.000001);
	}
	void testMoment1()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.75), stats.GetMoment((CLAM::O<1>*)0));
	}
	void testMoment2()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(9.75), stats.GetMoment((CLAM::O<2>*)0));
	}
	void testMoment3()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(40.25), stats.GetMoment((CLAM::O<3>*)0));
	}
	void testMoment4()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(180.75), stats.GetMoment((CLAM::O<4>*)0));
	}
	void testMoment5()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(850.25), stats.GetMoment((CLAM::O<5>*)0));
	}
	void testMoment6()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(4104.75), stats.GetMoment((CLAM::O<6>*)0));
	}
	void testSlope_withPrimesUp()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(0.118182), stats.GetSlope(), .000001);
	}
	void testSlope_withPrimesDown()
	{
		fillPrimesDown();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-0.118182), stats.GetSlope(), .000001);
	}
	void testSlope_withConstant()
	{
		fillConstant();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(0.), stats.GetSlope());
	}
	void testSlope_withInvertedUp()
	{
		fillPrimesUpInverted();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-0.126229), stats.GetSlope(), .000001);
	}
	void testTilt_withPrimesUp()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(2.8841e-10), stats.GetTilt() , .00001e-10);
	}
	void testTilt_withPrimesDown()
	{
		fillPrimesDown();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-1.92258e-07), stats.GetTilt() , .00001e-7);
	}
	void testTilt_withConstant()
	{
		fillConstant();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(1.3411e-07), stats.GetTilt(), .00001e-7);
	}
	void testTilt_withCountUp()
	{
		fillCountUp();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(1.28685e-07), stats.GetTilt(), .00001e-7);
	}
	void testTilt_withCountDown()
	{
		fillCountDown();
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( CLAM::TData(-2.81365e-07), stats.GetTilt(), .00001e-7);
	}
};




} // namespace CLAMTes
