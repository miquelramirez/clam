#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Stats.hxx" // CLAM

namespace CLAMTest
{



class StatsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( StatsTest );

class StatsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( StatsTest );
	CPPUNIT_TEST( testCentroid );
	CPPUNIT_TEST( testCentroid_withSingleDelta );
	CPPUNIT_TEST( testCentroid_withSingleDeltaAtTheFirstBin );
	CPPUNIT_TEST( testCentroid_withDeltasAtExtremes );
	CPPUNIT_TEST( testCentroid_withEvenZeros );
	CPPUNIT_TEST( testCentroid_withOddZeros );
	CPPUNIT_TEST( testCentroid_withEvenConstant );
	CPPUNIT_TEST( testCentroid_withOddConstant );
	CPPUNIT_TEST( testMean_withNoElements );
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
	CPPUNIT_TEST_SUITE_END();


	//@TODO: MRJ: Move to CppUnitHelper???
	void assertDoublesEqual( double given, double expected, double eps )
	{
		CPPUNIT_ASSERT( std::isnan(given) == std::isnan( expected ) );
		CPPUNIT_ASSERT( std::isinf( given ) == std::isinf( expected ) );

		CPPUNIT_ASSERT_DOUBLES_EQUAL( given, expected, eps);
	}


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
	void fillLongLinearSerie()
	{
		for (unsigned i=0; i<1000; i++)
		{
				_array.AddElem(CLAM::TData(i));
		}
		for (unsigned i=1000; i--; )
		{
				_array.AddElem(CLAM::TData(i));
		}
	}
	void fillZeros(unsigned int size)
	{
		for (unsigned i=0; i<size; i++)
		{
				_array.AddElem(CLAM::TData(0));
		}
	}
	void fillWith(unsigned int size, CLAM::TData value)
	{
		for (unsigned i=0; i<size; i++)
		{
				_array.AddElem(value);
		}
	}

	CLAM::Array<CLAM::TData> _array;

private:
	void testCentroid()
	{
		fillPrimesUp();
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(2.09091), stats.GetCentroid(), 0.000001);
	}

	void testCentroid_withSingleDelta()
	{
		fillZeros(50);
		_array[40]=CLAM::TData(1.0);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(40.0), stats.GetCentroid(), 0.000001);
	}
	
	void testCentroid_withSingleDeltaAtTheFirstBin()
	{
		fillZeros(50);
		_array[0]=CLAM::TData(1.0);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(0.0), stats.GetCentroid(), 0.000001);
	}
	

	void testCentroid_withEvenZeros()
	{
		fillZeros(50);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(24.5), stats.GetCentroid(), 0.000001);
	}

	void testCentroid_withOddZeros()
	{
		fillZeros(51);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(25.0), stats.GetCentroid(), 0.000001);
	}

	void testCentroid_withEvenConstant()
	{
		fillWith(50,2.0);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(24.5), stats.GetCentroid(), 0.000001);
	}

	void testCentroid_withOddConstant()
	{
		fillWith(51,2.0);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(25.0), stats.GetCentroid(), 0.000001);
	}

	void testCentroid_withDeltasAtExtremes()
	{
		fillZeros(50);
		_array[0]=CLAM::TData(1.0);
		_array[49]=CLAM::TData(1.0);
		CLAM::Stats stats(&_array);

		assertDoublesEqual( CLAM::TData(24.5), stats.GetCentroid(), 0.000001);
	}
	void testMean_withNoElements()
	{
		CLAM::Stats stats(&_array);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(0.0), stats.GetMean());
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
		assertDoublesEqual( CLAM::TData(0.118182), stats.GetSlope(), .000001);
	}
	void testSlope_withPrimesDown()
	{
		fillPrimesDown();
		CLAM::Stats stats(&_array);
		assertDoublesEqual( CLAM::TData(-0.118182), stats.GetSlope(), .000001);
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
		assertDoublesEqual( CLAM::TData(-0.126229), stats.GetSlope(), .000001);
	}
};




} // namespace CLAMTes

