#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Functors.hxx"
#include "DataTypes.hxx"


namespace CLAMTest
{

class FunctorsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FunctorsTest );

class FunctorsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FunctorsTest );
	CPPUNIT_TEST( testApplyUnaryFunctionAbs );
	CPPUNIT_TEST( testApplyUnaryFunctionNeg );
	CPPUNIT_TEST( testApplyUnaryComposition );
	CPPUNIT_TEST( testApplyUnaryCompositionInverted );
	CPPUNIT_TEST( testComposingWithApplyOperator );
	CPPUNIT_TEST( testComposingWithApplyOperatorInverted );
	CPPUNIT_TEST( testComposingCompositions );
	CPPUNIT_TEST( testPows );
	CPPUNIT_TEST( testApplyBinaryFunctionSum );
	CPPUNIT_TEST( testApplyBinaryFunctionProd );
	CPPUNIT_TEST( testApplyBinaryComposition );
	CPPUNIT_TEST( testApplyBinaryCompositionAppliedToSingleScalar );
	CPPUNIT_TEST( testApplyBinaryCompositionOnUnaryComposition );
	CPPUNIT_TEST( testApplySubstraction );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void assertArrayEquals(unsigned size, CLAM::TData * expected, CLAM::TData * result)
	{
		for (unsigned int i = 0; i<size; i++)
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i],result[i],.0001);
	}

	void testApplyUnaryFunctionAbs()
	{
		CLAM::abs myAbs;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(2.0),.0001);
	}

	void testApplyUnaryFunctionNeg()
	{
		CLAM::neg myNeg;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myNeg(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(2.0),.0001);
	}

	void testApplyUnaryComposition()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, CLAM::compose (myNeg,myAbs)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, CLAM::compose (myNeg,myAbs)(2.0),.0001);
	}

	void testApplyUnaryCompositionInverted()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, CLAM::compose (myAbs,myNeg)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, CLAM::compose (myAbs,myNeg)(2.0),.0001);
	}

	void testComposingWithApplyOperator()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(myAbs)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-2, myNeg(myAbs)(2.0),.0001);
	}

	void testComposingWithApplyOperatorInverted()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(myNeg)(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2, myAbs(myNeg)(2.0),.0001);
	}

	void testComposingCompositions()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;
		CLAM::cubed myCubed;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(-8, myCubed(myNeg(myAbs))(-2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-8, myCubed(myNeg(myAbs))(2.0),.0001);
	}

	void testPows()
	{
		CLAM::pow<0> pow0;
		CLAM::pow<1> identity;
		CLAM::pow<2> squared;
		CLAM::pow<3> cubed;
		CLAM::pow<4> pow4;
		CLAM::pow<5> pow5;
		CLAM::pow<6> pow6;
		CLAM::pow<7> pow7;
		CLAM::pow<8> pow8;
		CLAM::pow<9> pow9;
		CLAM::pow<37> pow37;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, pow0(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  2, identity(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  4, squared(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(  8, cubed(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 16, pow4(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 32, pow5(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 64, pow6(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(128, pow7(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(256, pow8(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(512, pow9(2.0),.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(137438953472., pow37(2.0),.0001);
	}

	void testApplyBinaryFunctionSum()
	{
		CLAM::sum mySum;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(3, mySum(-2.0,5.0),.0001);
	}

	void testApplyBinaryFunctionProd()
	{
		CLAM::prod myProd;
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-10, myProd(-2.0,5.0),.0001);
	}

	void testApplyBinaryComposition()
	{
		CLAM::neg myNeg;
		CLAM::abs myAbs;
		CLAM::prod myProd;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(14, myProd(myNeg,myAbs)(-2.0,-7.0),.0001);
	}

	void testApplyBinaryCompositionAppliedToSingleScalar()
	{
		CLAM::neg myNeg;
		CLAM::sum mySum;
		CLAM::pow<2> myPow2;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(6, mySum(myPow2,myNeg)(3.0),.0001);
	}

	void testApplyBinaryCompositionOnUnaryComposition()
	{
		CLAM::abs myAbs;
		CLAM::neg myNeg;
		CLAM::sum mySum;
		CLAM::pow<3> myPow3;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(5, mySum(myPow3,myNeg(myAbs))(2.0,-3.0),.0001);
	}

	void testApplySubstraction()
	{
		CLAM::subst mySubst;

		CPPUNIT_ASSERT_DOUBLES_EQUAL(5, mySubst(2.0,-3.0),.0001);
	}

};






} // namespace CLAMTest
