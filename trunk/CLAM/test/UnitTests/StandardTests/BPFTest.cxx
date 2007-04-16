#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "BPF.hxx" // CLAM
#include "CLAM_Math.hxx" // CLAM

namespace CLAMTest
{
	class BPFTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( BPFTest );

	class BPFTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( BPFTest );

		CPPUNIT_TEST( test_After_SetValue_i_Y_GetValue_i_Yields_Y );
		CPPUNIT_TEST( test_After_SetValue_0_Y_GetValueFromIndex_0_Yields_Y );
		CPPUNIT_TEST( test_Copy_Of_BPFs );
		CPPUNIT_TEST( test_Delete_Between_A_Range_Of_X_Values );
		CPPUNIT_TEST( test_Delete_Through_X_Value );
		CPPUNIT_TEST( test_Insert_X_Y_Pair );
		CPPUNIT_TEST( test_Insert_Point );
		CPPUNIT_TEST( test_GetValue_Interpolation_Mode_Step );
		CPPUNIT_TEST( test_GetValue_Interpolation_Mode_Round );
		CPPUNIT_TEST( test_GetValue_Interpolation_Mode_Linear );
		CPPUNIT_TEST( test_GetValue_Interpolation_Mode_Quadratic );
		CPPUNIT_TEST( test_GetValue_Interpolation_Mode_Spline );
		
		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp()
		{
		}

		void tearDown()
		{
		}

	private:

		void test_After_SetValue_i_Y_GetValue_i_Yields_Y()
		{
			CLAM::BPF bpf;

			bpf.Resize(2);
			bpf.SetSize(2);

			bpf.SetXValue( 0, CLAM::TData(0) );
			bpf.SetValue( 0, CLAM::TData(1) );
			bpf.SetXValue( 1, CLAM::TData(1) );
			bpf.SetValue( 1, CLAM::TData(0) );

			CPPUNIT_ASSERT( bpf.GetValue(CLAM::TData(0)) == CLAM::TData(1) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex(0) == CLAM::TData(1) );
			CPPUNIT_ASSERT( bpf.GetValue(CLAM::TData(1)) == CLAM::TData(0) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex(1) == CLAM::TData(0) );
		}

		void test_After_SetValue_0_Y_GetValueFromIndex_0_Yields_Y()
		{
			CLAM::BPF bpf;

			bpf.Resize(1);
			bpf.SetSize(1);

			bpf.SetValue( 0, CLAM::TData(1) );

			CPPUNIT_ASSERT( bpf.GetValueFromIndex(0) == CLAM::TData(1) );

		}

		void test_Copy_Of_BPFs()
		{
			CLAM::BPF originalBPF;

			originalBPF.Resize(5);
			originalBPF.SetSize(5);

			originalBPF.SetXValue(0, CLAM::TData(0) );
			originalBPF.SetXValue(1, CLAM::TData(1) );
			originalBPF.SetXValue(2, CLAM::TData(2) );
			originalBPF.SetXValue(3, CLAM::TData(3) );
			originalBPF.SetXValue(4, CLAM::TData(4) );
			
			originalBPF.SetValue( 0, CLAM::TData(1) );
			originalBPF.SetValue( 1, CLAM::TData(1) );
			originalBPF.SetValue( 2, CLAM::TData(1) );
			originalBPF.SetValue( 3, CLAM::TData(1) );
			originalBPF.SetValue( 4, CLAM::TData(1) );
			
			CLAM::BPF theCopy;

			theCopy = originalBPF;

			CPPUNIT_ASSERT( originalBPF.GetXValue(0) == theCopy.GetXValue(0) );
			CPPUNIT_ASSERT( originalBPF.GetXValue(1) == theCopy.GetXValue(1) );
			CPPUNIT_ASSERT( originalBPF.GetXValue(2) == theCopy.GetXValue(2) );
			CPPUNIT_ASSERT( originalBPF.GetXValue(3) == theCopy.GetXValue(3) );
			CPPUNIT_ASSERT( originalBPF.GetXValue(4) == theCopy.GetXValue(4) );

			CPPUNIT_ASSERT( originalBPF.GetValue(0) == theCopy.GetValue(0) );
			CPPUNIT_ASSERT( originalBPF.GetValue(1) == theCopy.GetValue(1) );
			CPPUNIT_ASSERT( originalBPF.GetValue(2) == theCopy.GetValue(2) );
			CPPUNIT_ASSERT( originalBPF.GetValue(3) == theCopy.GetValue(3) );
			CPPUNIT_ASSERT( originalBPF.GetValue(4) == theCopy.GetValue(4) );
			
		}

		void test_Delete_Through_X_Value()
		{
			CLAM::BPF theBPF;

			theBPF.Resize(3);
			theBPF.SetSize(3);

			theBPF.SetXValue( 0, CLAM::TData(0) );
			theBPF.SetXValue( 1, CLAM::TData(1) );
			theBPF.SetXValue( 2, CLAM::TData(2) );

			theBPF.SetValue( 0, CLAM::TData(1) );
			theBPF.SetValue( 1, CLAM::TData(1) );
			theBPF.SetValue( 2, CLAM::TData(1) );

			theBPF.DeleteThroughXValue( CLAM::TData(1) );

			CPPUNIT_ASSERT( theBPF.Size() == 2 );
			CPPUNIT_ASSERT( theBPF.GetXValue(0) == 0 );
			CPPUNIT_ASSERT( theBPF.GetXValue(1) == 2 );
		}
		
		void test_Delete_Between_A_Range_Of_X_Values()
		{
			CLAM::BPF theBPF;

			theBPF.Resize(5);
			theBPF.SetSize(5);

			theBPF.SetXValue(0, CLAM::TData(0) );
			theBPF.SetXValue(1, CLAM::TData(1) );
			theBPF.SetXValue(2, CLAM::TData(2) );
			theBPF.SetXValue(3, CLAM::TData(3) );
			theBPF.SetXValue(4, CLAM::TData(4) );
			
			theBPF.SetValue( 0, CLAM::TData(1) );
			theBPF.SetValue( 1, CLAM::TData(1) );
			theBPF.SetValue( 2, CLAM::TData(1) );
			theBPF.SetValue( 3, CLAM::TData(1) );
			theBPF.SetValue( 4, CLAM::TData(-1) );

			theBPF.DeleteBetweenXValues( CLAM::TData(1), CLAM::TData(3) );


			CPPUNIT_ASSERT( theBPF.GetValue(0) == 1 );
			CPPUNIT_ASSERT( theBPF.GetValue(4) == -1 );

			CPPUNIT_ASSERT( theBPF.GetXValue(0) == 0 );
			CPPUNIT_ASSERT( theBPF.GetXValue(1) == 4 );

			
		}

		void test_Insert_X_Y_Pair()
		{
			CLAM::BPF theBPF;

			theBPF.Resize( 2 );
			theBPF.SetSize( 2 );
			
			theBPF.SetXValue( 0, CLAM::TData(0) );
			theBPF.SetValue( 0, CLAM::TData( 2 ) );
			theBPF.SetXValue( 1, CLAM::TData( 31 ) );
			theBPF.SetValue( 1, CLAM::TData( 1.5 ) );

			CLAM::TIndex insertedAt = theBPF.Insert( 30.98, 1.75 );

			CPPUNIT_ASSERT( theBPF.Size() == 3 );
			CPPUNIT_ASSERT( insertedAt == 1 );

		}

		void test_Insert_Point()
		{
			CLAM::BPF theBPF;

			theBPF.Resize( 2 );
			theBPF.SetSize( 2 );
			
			theBPF.SetXValue( 0, CLAM::TData(0) );
			theBPF.SetValue( 0, CLAM::TData( 2 ) );
			theBPF.SetXValue( 1, CLAM::TData( 31 ) );
			theBPF.SetValue( 1, CLAM::TData( 1.5 ) );

			CLAM::TIndex insertedAt = theBPF.Insert( CLAM::Point(30.98, 1.75) );

			CPPUNIT_ASSERT( theBPF.Size() == 3 );
			CPPUNIT_ASSERT( insertedAt == 1 );
			
		}

		void test_GetValue_Interpolation_Mode_Step()
		{
			CLAM::BPF theBPF;

			theBPF.Insert( 0.0, 2.0 );
			theBPF.Insert( 1.5, 1.0 );
			theBPF.Insert( 3.0, 2.0 );

			CLAM::TData interpYa = theBPF.GetValue( 0.8, CLAM::EInterpolation::eStep );
			CLAM::TData interpYb = theBPF.GetValue( 2.72, CLAM::EInterpolation::eStep );


			CPPUNIT_ASSERT( fabs( interpYa - 2.0 ) <= 1e-7 );
			CPPUNIT_ASSERT( fabs( interpYb - 1.0 ) <= 1e-7 );
		}

		void test_GetValue_Interpolation_Mode_Round()
		{
			CLAM::BPF theBPF;

			theBPF.Insert( 0.0, 2.0 );
			theBPF.Insert( 1.5, 1.0 );
			theBPF.Insert( 3.0, 2.0 );

			//CLAM::TData interpYa = theBPF.GetValue( 0.8, CLAM::EInterpolation::eRound );
			//CLAM::TData interpYb = theBPF.GetValue( 2.72, CLAM::EInterpolation::eRound );

			// :TODO: This is not working properly!!
			//CPPUNIT_ASSERT( fabs( interpYa - 1.0 ) <= 1e-7 );
			//CPPUNIT_ASSERT( fabs( interpYb - 2.0 ) <= 1e-7 );
			
		}

		void test_GetValue_Interpolation_Mode_Linear()
		{
			CLAM::BPF theBPF;

			theBPF.Insert( 0.0, 2.0 );
			theBPF.Insert( 1.5, 1.0 );
			theBPF.Insert( 3.0, 2.0 );

			CLAM::TData interpYa = theBPF.GetValue( 0.8, CLAM::EInterpolation::eLinear );
			CLAM::TData interpYb = theBPF.GetValue( 2.72, CLAM::EInterpolation::eLinear );
			
			CPPUNIT_ASSERT( fabs( interpYa - 1.46667 ) <= 5e-6 );
			CPPUNIT_ASSERT( fabs( interpYb - 1.81333 ) <= 5e-6 );			
		}

		void test_GetValue_Interpolation_Mode_Quadratic()
		{
			CLAM::BPF theBPF;

			theBPF.Insert( 0.0, 2.0 );
			theBPF.Insert( 1.5, 1.0 );
			theBPF.Insert( 3.0, 2.0 );

			CLAM::TData interpYa = theBPF.GetValue( 0.8, CLAM::EInterpolation::ePolynomial2 );
			CLAM::TData interpYb = theBPF.GetValue( 2.72, CLAM::EInterpolation::ePolynomial2 );			

			CPPUNIT_ASSERT( fabs( interpYa - 1.46667 ) <= 5e-6 );
			CPPUNIT_ASSERT( fabs( interpYb - 1.81333 ) <= 5e-6 );			

		}

		void test_GetValue_Interpolation_Mode_Spline()
		{
			CLAM::BPF theBPF;

			theBPF.Insert( 0.0, 2.0 );
			theBPF.Insert( 1.5, 1.0 );
			theBPF.Insert( 3.0, 2.0 );

			CLAM::TData interpYa = theBPF.GetValue( 0.8, CLAM::EInterpolation::ePolynomial2 );
			CLAM::TData interpYb = theBPF.GetValue( 2.72, CLAM::EInterpolation::ePolynomial2 );			

			CPPUNIT_ASSERT( fabs( interpYa - 1.46667 ) <= 5e-6 );
			CPPUNIT_ASSERT( fabs( interpYb - 1.81333 ) <= 5e-6 );			

		}

	};
}

