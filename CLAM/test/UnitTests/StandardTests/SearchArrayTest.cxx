#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "SearchArray.hxx" // CLAM
#include "stl_optbinsearch.h" // CLAM
#include "Array.hxx" // CLAM
#include "Point.hxx" // CLAM

namespace CLAM_Math
{
	struct compareX
	{
		bool operator()( const CLAM::Point& lhs, const CLAM::Point& rhs )
		{
			return ( lhs.GetX() >= rhs.GetX() );
		}
	};

	class SearchArrayTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( SearchArrayTest );
	
	class SearchArrayTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SearchArrayTest );

		CPPUNIT_TEST( test_Search_On_Ordered_Array_Of_Points );
		CPPUNIT_TEST( test_optbinsearch );
		CPPUNIT_TEST( test_optbinsearch_w_pred );

		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp()
		{
		}

		void tearDown()
		{
		}

	private:

		void test_Search_On_Ordered_Array_Of_Points()
		{
			CLAM::Array< CLAM::Point > array;

			CLAM::SearchArray< CLAM::Point > searchArray;

			array.AddElem( CLAM::Point( 0.0, 2.0 ) );
			array.AddElem( CLAM::Point( 1.5, 1.0 ) );
			array.AddElem( CLAM::Point( 3.0, 2.0 ) );

			searchArray.Set( array );

			CLAM::TIndex nearest = searchArray.Find( CLAM::Point( 0.8, 0.0 ), -1 );

			CPPUNIT_ASSERT( nearest == 0 );

			nearest = searchArray.Find( CLAM::Point( -3.2, 0.0 ), nearest );
			
			CPPUNIT_ASSERT( nearest == -1 );

			nearest = searchArray.Find( CLAM::Point( 3.2, 0.0 ), nearest );

			CPPUNIT_ASSERT( nearest == 2 );
		}

		void test_optbinsearch()
		{
			CLAM::Array< CLAM::Point > array;

			array.AddElem( CLAM::Point( 0.0, 2.0 ) );
			array.AddElem( CLAM::Point( 1.5, 1.0 ) );
			array.AddElem( CLAM::Point( 3.0, 2.0 ) );
			
			CLAM::Point* nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
							  CLAM::Point( 0.8, 0.0 ) );

			CPPUNIT_ASSERT( std::distance( nearest, array.GetPtr() ) == 0 );

			nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
					     CLAM::Point( -3.2, 0.0 ), nearest );
			
			CPPUNIT_ASSERT( nearest == array.GetPtr()+array.Size() );

			nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
					     CLAM::Point( 3.2, 0.0 ),  nearest );


			CPPUNIT_ASSERT( nearest ==  array.GetPtr()+2 );
		}

		void test_optbinsearch_w_pred()
		{
			CLAM::Array< CLAM::Point > array;

			array.AddElem( CLAM::Point( 0.0, 2.0 ) );
			array.AddElem( CLAM::Point( 1.5, 1.0 ) );
			array.AddElem( CLAM::Point( 3.0, 2.0 ) );
			
			CLAM::Point* nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
							  CLAM::Point( 0.8, 0.0 ), compareX() );

			CPPUNIT_ASSERT( std::distance( nearest, array.GetPtr() ) == 0 );

			nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
					     CLAM::Point( -3.2, 0.0 ), nearest, compareX() );
			
			CPPUNIT_ASSERT( nearest == array.GetPtr()+array.Size() );

			nearest = std::hunt( array.GetPtr(), array.GetPtr()+array.Size(),
					     CLAM::Point( 3.2, 0.0 ),  nearest, compareX() );


			CPPUNIT_ASSERT( nearest ==  array.GetPtr()+2 );

		}
	};


}

