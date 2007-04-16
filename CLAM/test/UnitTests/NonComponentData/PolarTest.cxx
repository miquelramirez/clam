#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Polar.hxx" // CLAM

namespace CLAMTest
{


class PolarTest;

CPPUNIT_TEST_SUITE_REGISTRATION( PolarTest );

class PolarTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PolarTest );
	CPPUNIT_TEST( testConstructor_TakesValues );
	CPPUNIT_TEST( testDefaultConstructor_InitAsZero );
	CPPUNIT_TEST( testSetAng );
	CPPUNIT_TEST( testSetMag );
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
		CLAM::Polar aPolar(3.0, 2.0);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(3.0), aPolar.Mag() );
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.0), aPolar.Ang() );
	}

	void testDefaultConstructor_InitAsZero()
	{
		CLAM::Polar zeroPolar(0.0, 0.0);
		CLAM::Polar aDefaultConstructedPolar;

		CPPUNIT_ASSERT_EQUAL(aDefaultConstructedPolar, zeroPolar);
	}

	void testInequality_WithDifferentValues()
	{
		CLAM::Polar pureMagUnitPolar(1.0, 0.0);
		CLAM::Polar pureAngUnitPolar(0.0, 1.0);

		CPPUNIT_ASSERT(pureAngUnitPolar != pureMagUnitPolar);
	}

	void testSetAng()
	{
		CLAM::Polar aPolar(3.0, 2.0);
		aPolar.SetAng(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Polar(3.0,4.3), aPolar);
	}

	void testSetMag()
	{
		CLAM::Polar aPolar(3.0, 2.0);
		aPolar.SetMag(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Polar(4.3,2.0), aPolar);
	}


	void testStreamInsertion()
	{
		std::stringstream s;
		CLAM::Polar polar(1.453,3.454);
		std::string expectedString("{1.453 3.454}");

		s << polar << std::flush;

		CPPUNIT_ASSERT_EQUAL(expectedString, s.str());
	}

	void testStreamExtraction()
	{
		CLAM::Polar toBeModified(1,3);
		std::string inputString("{5.3 7.3}");
		CLAM::Polar expected(5.3, 7.3);
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithSpacesToBeIgnored()
	{
		CLAM::Polar toBeModified(1,3);
		std::string inputString(" \n  { \n 5.3 \n  7.3 \t } ");
		CLAM::Polar expected(5.3, 7.3);
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentStartToken()
	{
		CLAM::Polar toBeModified(1.3,3.2);
		CLAM::Polar expected    (1.3,3.2);
		std::string inputString("a{5.3 7.3}");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentEndToken()
	{
		CLAM::Polar toBeModified(1.3,3.2);
		CLAM::Polar expected    (1.3,3.2);
		std::string inputString("{5.3 7.3a a");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

};




} // namespace CLAMTes

