#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Complex.hxx" // CLAM

namespace CLAMTest
{


class ComplexTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ComplexTest );

class ComplexTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ComplexTest );
	CPPUNIT_TEST( testConstructor_TakesValues );
	CPPUNIT_TEST( testDefaultConstructor_InitAsZero );
	CPPUNIT_TEST( testSetImag );
	CPPUNIT_TEST( testSetReal );
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
		CLAM::Complex aComplex(3.0, 2.0);
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(3.0), aComplex.Real());
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(2.0), aComplex.Imag());
	}

	void testDefaultConstructor_InitAsZero()
	{
		CLAM::Complex zeroComplex(0.0, 0.0);
		CLAM::Complex aDefaultConstructedComplex;

		CPPUNIT_ASSERT_EQUAL(aDefaultConstructedComplex, zeroComplex);
	}

	void testInequality_WithDifferentValues()
	{
		CLAM::Complex pureRealUnitComplex(1.0, 0.0);
		CLAM::Complex pureImagUnitComplex(0.0, 1.0);

		CPPUNIT_ASSERT(pureImagUnitComplex != pureRealUnitComplex);
	}

	void testSetImag()
	{
		CLAM::Complex aComplex(3.0, 2.0);
		aComplex.SetImag(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Complex(3.0,4.3), aComplex);
	}

	void testSetReal()
	{
		CLAM::Complex aComplex(3.0, 2.0);
		aComplex.SetReal(4.3);
		CPPUNIT_ASSERT_EQUAL(CLAM::Complex(4.3,2.0), aComplex);
	}


	void testStreamInsertion()
	{
		std::stringstream s;
		CLAM::Complex complex(1.453,3.454);
		std::string expectedString("{1.453 3.454i}");

		s << complex << std::flush;

		CPPUNIT_ASSERT_EQUAL(expectedString, s.str());
	}

	void testStreamExtraction()
	{
		CLAM::Complex toBeModified(1,3);
		std::string inputString("{5.4 7.4i}");
		CLAM::Complex expected( CLAM::TData(5.4), CLAM::TData(7.4) );
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithSpacesToBeIgnored()
	{
		CLAM::Complex toBeModified(1,3);
		std::string inputString(" \n  { \n 5.3 \n  7.3 i \t } ");
		CLAM::Complex expected(5.3, 7.3);
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentStartToken()
	{
		CLAM::Complex toBeModified(1.3,3.2);
		CLAM::Complex expected    (1.3,3.2);
		std::string inputString("a{5.3 7.3i}");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

	void testStreamExtraction_WithADifferentEndToken()
	{
		CLAM::Complex toBeModified(1.3,3.2);
		CLAM::Complex expected    (1.3,3.2);
		std::string inputString("{5.3 7.3ia a");
		std::stringstream ss(inputString);

		ss >> toBeModified;

		CPPUNIT_ASSERT_EQUAL(expected, toBeModified);
	}

};




} // namespace CLAMTest

