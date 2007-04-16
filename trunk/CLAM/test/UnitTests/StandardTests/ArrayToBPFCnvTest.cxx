#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "Array.hxx" // CLAM
#include "BPF.hxx" // CLAM
#include "ArrayToBPFCnv.hxx" // CLAM

namespace CLAMTest
{
	class ArrayToBPFCnvTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( ArrayToBPFCnvTest );
	
	class ArrayToBPFCnvTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( ArrayToBPFCnvTest );
		
		CPPUNIT_TEST( test_ConvertToBPF_NoXValuesSpecified );
		CPPUNIT_TEST( test_ConvertToBPF_XValuesSpecifiedAsStartAndDelta );
		CPPUNIT_TEST( test_ConvertToBPF_XValuesInsideAnArray );

		CPPUNIT_TEST_SUITE_END();

	protected:

		CLAM::Array<CLAM::TData> mTestXValues;
		CLAM::Array<CLAM::TData> mTestYValues;

	public:
		void setUp()
		{
		}

		void tearDown()
		{
		}

	private:
		
		void test_ConvertToBPF_NoXValuesSpecified()
		{

			CLAM::BPF bpf;

			CLAM::TData _xValues[] = {0, 100, 200, 300, 400, 500};
			CLAM::TData _yValues[] = {1, 0, 1, 0, 1, 0 };

			mTestXValues.SetPtr( _xValues, 6 );
			mTestYValues.SetPtr( _yValues, 6 );


			bpf.Resize( 6 );
			bpf.SetSize( 6 );

			CLAM::ConvertToBPF( bpf, mTestYValues ); 

			CPPUNIT_ASSERT( bpf.GetValue( 0 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 1 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 2 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 3 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 4 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 5 ) == CLAM::TData( 0 ) );

		}

		void test_ConvertToBPF_XValuesSpecifiedAsStartAndDelta()
		{
			CLAM::BPF bpf;

			CLAM::TData _xValues[] = {0, 100, 200, 300, 400, 500};
			CLAM::TData _yValues[] = {1, 0, 1, 0, 1, 0 };

			mTestXValues.SetPtr( _xValues, 6 );
			mTestYValues.SetPtr( _yValues, 6 );

			
			bpf.Resize(6);
			bpf.SetSize( 6 );

			CLAM::ConvertToBPF( bpf, CLAM::TData(0), CLAM::TData(100), mTestYValues );

			CPPUNIT_ASSERT( bpf.GetXValue( 0 ) == CLAM::TData(0) );
			CPPUNIT_ASSERT( bpf.GetXValue( 1 ) == CLAM::TData(100) );
			CPPUNIT_ASSERT( bpf.GetXValue( 2 ) == CLAM::TData(200) );
			CPPUNIT_ASSERT( bpf.GetXValue( 3 ) == CLAM::TData(300) );
			CPPUNIT_ASSERT( bpf.GetXValue( 4 ) == CLAM::TData(400) );
			CPPUNIT_ASSERT( bpf.GetXValue( 5 ) == CLAM::TData(500) );

			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 0 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 1 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 2 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 3 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 4 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 5 ) == CLAM::TData( 0 ) );


		}

		void test_ConvertToBPF_XValuesInsideAnArray()
		{

			CLAM::BPF bpf;
			bpf.Resize(6);
			bpf.SetSize(6);

			CLAM::TData _xValues[] = {0, 100, 200, 300, 400, 500};
			CLAM::TData _yValues[] = {1, 0, 1, 0, 1, 0 };

			mTestXValues.SetPtr( _xValues, 6 );
			mTestYValues.SetPtr( _yValues, 6 );


			CLAM::ConvertToBPF( bpf, mTestXValues, mTestYValues );

			CPPUNIT_ASSERT( bpf.GetXValue( 0 ) == CLAM::TData(0) );
			CPPUNIT_ASSERT( bpf.GetXValue( 1 ) == CLAM::TData(100) );
			CPPUNIT_ASSERT( bpf.GetXValue( 2 ) == CLAM::TData(200) );
			CPPUNIT_ASSERT( bpf.GetXValue( 3 ) == CLAM::TData(300) );
			CPPUNIT_ASSERT( bpf.GetXValue( 4 ) == CLAM::TData(400) );
			CPPUNIT_ASSERT( bpf.GetXValue( 5 ) == CLAM::TData(500) );

			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 0 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 1 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 2 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 3 ) == CLAM::TData( 0 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 4 ) == CLAM::TData( 1 ) );
			CPPUNIT_ASSERT( bpf.GetValueFromIndex( 5 ) == CLAM::TData( 0 ) );


		}

	};

}

