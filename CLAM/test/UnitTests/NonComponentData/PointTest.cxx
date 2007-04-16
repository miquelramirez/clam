#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Point.hxx" // CLAM

namespace CLAMTest
{



class PointTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PointTest );

class PointTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PointTest );
	CPPUNIT_TEST( testConstructor_TakesValues );
	CPPUNIT_TEST( testDefaultConstructor_InitAsZero );
	CPPUNIT_TEST( testSetY );
	CPPUNIT_TEST( testSetX );
	CPPUNIT_TEST( testInequality_WithDifferentValues );
	CPPUNIT_TEST( testStreamInsertion );
	CPPUNIT_TEST( testStreamExtraction );
	CPPUNIT_TEST( testStreamExtraction_WithSpacesToBeIgnored );
	CPPUNIT_TEST( testStreamExtraction_WithADifferentStartToken );
	CPPUNIT_TEST( testStreamExtraction_WithADifferentEndToken );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testConstructor_TakesValues()
	{
		CLAM::Point aPoint(3.0, 2.0);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(3.0), aPoint.GetX());
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.0), aPoint.GetY());
	}

	void testDefaultConstructor_InitAsZero()
	{
		CLAM::Point zeroPoint(0.0, 0.0);
		CLAM::Point aDefaultConstructedPoint;

		CPPUNIT_ASSERT_EQUAL(aDefaultConstructedPoint, zeroPoint);
	}

	void testInequality_WithDifferentValues()
	{
		CLAM::Point pureXUnitPoint(1.0, 0.0);
		CLAM::Point pureYUnitPoint(0.0, 1.0);

		CPPUNIT_ASSERT(pureYUnitPoint != pureXUnitPoint);
	}

	void testSetY()
	{
		CLAM::Point aPoint(3.0, 2.0);
		aPoint.SetY(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Point(3.0,4.3), aPoint);
	}

	void testSetX()
	{
		CLAM::Point aPoint(3.0, 2.0);
		aPoint.SetX(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Point(4.3,2.0), aPoint);
	}


	void testStreamInsertion()
	{
		std::stringstream s;
		CLAM::Point point(1.453,3.454);
		std::string expectedString("{1.453 3.454}");

		s << point << std::flush;

		CPPUNIT_ASSERT_EQUAL(expectedString, s.str());
	}

	void testStreamExtraction()
	{
		CLAM::Point toBeModified(1,3);
		std::string inputString("{5.3 7.3}");
		CLAM::Point expected(5.3, 7.3);
		std::stringstream s(inputString);

		s >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithSpacesToBeIgnored()
	{
		CLAM::Point toBeModified(1,3);
		std::string inputString(" \n  { \n 5.3 \n  7.3 \t } ");
		CLAM::Point expected(5.3, 7.3);
		std::stringstream s(inputString);

		s >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentStartToken()
	{
		CLAM::Point toBeModified(1.3,3.2);
		CLAM::Point expected    (1.3,3.2);
		std::string inputString("a{5.3 7.3}");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentEndToken()
	{
		CLAM::Point toBeModified(1.3,3.2);
		CLAM::Point expected    (1.3,3.2);
		std::string inputString("{5.3 7.3a a");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

};




} // namespace CLAMTes

