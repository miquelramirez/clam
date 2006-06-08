#include "WritingRegion.hxx" // CLAM
#include "ReadingRegion.hxx" // CLAM

#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <list>
#include "PhantomBuffer.hxx" // CLAM
#include "DataTypes.hxx" // CLAM

namespace CLAMTest {

// Test suite registration at the end of file
// That's due to the fact that we register template specialization of the suite tmpl class
//
template <template <class> class DataStructure>
class TestsStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsStream );
	
	CPPUNIT_TEST( testWritingRegion_constructor );
	CPPUNIT_TEST( testReadingRegion_constructor );
	CPPUNIT_TEST( testWritingRegion_canProduce_withNoReaders );
	CPPUNIT_TEST( testWritingRegion_canProduce_withOneReader );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter );
	CPPUNIT_TEST( testWritingRegion_fulfilsInvariant_whenReaderSurpassesWriterWithoutOverlapping );
	CPPUNIT_TEST( testReadingRegion_canConsume_whenRegionsAreOverlapped );
	CPPUNIT_TEST( testReadingRegion_canConsume_when2ReadingRegionsBehindAndNonOverlap );
	CPPUNIT_TEST( testWritingRegion_produce_movesRegion );
	CPPUNIT_TEST( testReadingRegion_consume_whenIsLegal_movesRegion );
	CPPUNIT_TEST( testProduceConsumeData_withUnitaryRegions );
	CPPUNIT_TEST( testProduceConsumeData_withSizedRegions );
	CPPUNIT_TEST( testProduceConsumeData_withSizedRegions_writerProducesMultipleTimes );
	CPPUNIT_TEST( testStreamImplementation_avancingALongWay_semiStressTest );
	CPPUNIT_TEST( testWritingRegion_CenterRegion_withOneReader );
	CPPUNIT_TEST( testWritingRegion_CenterRegion_withSomeReaders );
	CPPUNIT_TEST( testWritingRegion_CenterRegion_assertsWithUnevenRegion );
	CPPUNIT_TEST( testWritingRegion_CenterRegion_withOneReaderAndBiggerWriter );
	CPPUNIT_TEST( testWritingRegion_GetLastWrittenData_withoutOverlap );
	CPPUNIT_TEST( testWritingRegion_GetLastWrittenData_withOverlap );
	CPPUNIT_TEST( testWritingRegion_GetLastWrittenData_withDifferentHopAndSize );
	CPPUNIT_TEST_SUITE_END();

	void testWritingRegion_constructor()
	{
		CLAM::WritingRegion<int,DataStructure> writer;
		CPPUNIT_ASSERT(0 == writer.Pos());
		CPPUNIT_ASSERT(1 == writer.Size());
		CPPUNIT_ASSERT(1 == writer.Hop());
	}

	void testReadingRegion_constructor()
	{
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		CPPUNIT_ASSERT(0 == reader.Pos());
		CPPUNIT_ASSERT(1 == reader.Size());
		CPPUNIT_ASSERT(1 == reader.Hop());
	}

	void testWritingRegion_canProduce_withNoReaders()
	{
		CLAM::WritingRegion<int,DataStructure> writer;
		CPPUNIT_ASSERT( true == writer.CanProduce() );
	}

	void testWritingRegion_canProduce_withOneReader()
	{
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );
		CPPUNIT_ASSERT( true == writer.CanProduce() );
	}

	void testWritingRegion_fulfilsInvariant_whenRegionsAreOverlapped()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:  ^
		// W:  ^
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;

		writer.LinkRegions( reader );

		CPPUNIT_ASSERT( true == writer.FulfilsInvariant() );
	}

	void testWritingRegion_fulfilsInvariant_whenReaderOverlapsAndSurpassesWriter()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:   ^--^
		// W:  ^^
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		writer.Size(2);
		reader.Pos(1);
		reader.Size(4);

		CPPUNIT_ASSERT( true == writer.FulfilsInvariant() );
	}

	void testWritingRegion_fulfilsInvariant_whenReaderSurpassesWriterWithoutOverlapping()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:      ^--^
		// W:  ^--^
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		writer.Size(4);
		reader.Size(4);
		reader.Pos(4);

		CPPUNIT_ASSERT( false == writer.FulfilsInvariant() );
	}


	void testReadingRegion_canConsume_whenRegionsAreOverlapped()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R:  ^
		// W:  ^
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		CPPUNIT_ASSERT( false == reader.CanConsume() );
	}

	void testReadingRegion_canConsume_when2ReadingRegionsBehindAndNonOverlap()
	{
		//    |0         1         2
		//    |012345678901234567890
		// R1: ^
		// R2: ^-^
		// W:     ^
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader1;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader2;
		writer.LinkRegions( reader1 );
		writer.LinkRegions( reader2 );

		reader2.Size(3);
		writer.Pos(3);

		CPPUNIT_ASSERT( true == reader2.CanConsume() );
		CPPUNIT_ASSERT( true == reader1.CanConsume() );
	}


	void testWritingRegion_produce_movesRegion()
	{
		CLAM::WritingRegion<int,DataStructure> writer;
		writer.Size(4);
		writer.Hop(3);
		writer.Produce();
		CPPUNIT_ASSERT_EQUAL( long(3), writer.Pos() );
	}

	void testReadingRegion_consume_whenIsLegal_movesRegion()
	{
		//         |0         1         2
		//         |012345678901234567890
		// R_init:  ^--^
		// R_final:   ^--^
		// W:           ^-^
		//

		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		writer.Size(3);
		writer.Pos(4);
		reader.Size(4);
		reader.Hop(2);

		reader.Consume();

		CPPUNIT_ASSERT_EQUAL( long(2), reader.Pos() );
		CPPUNIT_ASSERT( false == reader.CanConsume() );
	}

	void testProduceConsumeData_withUnitaryRegions()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		writer[0] = 'a';
		writer.Produce();
		CPPUNIT_ASSERT_EQUAL( 'a', reader[0] );
		reader.Consume();
		CPPUNIT_ASSERT( false == reader.CanConsume() );
	}

	void testProduceConsumeData_withSizedRegions()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		writer.Size(5);
		writer.Hop(5);
		CPPUNIT_ASSERT_EQUAL( 0, int(writer.Pos()) );


		writer[0] = 'h';
		writer[1] = 'e';
		writer[2] = 'l';
		writer[3] = 'l';
		writer[4] = 'o';
		writer.Produce();

		reader.Size(3);
		reader.Hop(2);
		CPPUNIT_ASSERT_EQUAL( 'h', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'e', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'l', reader[2] );

		reader.Consume();
		CPPUNIT_ASSERT_EQUAL( 'l', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'l', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'o', reader[2] );

		reader.Consume();
		CPPUNIT_ASSERT( false == reader.CanConsume() );
	}

	void testProduceConsumeData_withSizedRegions_writerProducesMultipleTimes()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );

		//       	|hello world
		// W         ^--------^  (hop=5)
		// R		 ^-^         (hop=2)
		//#consume
		//   #produce
		//    1		      ^-------^
		// 1		   ^-^
		// 2		     ^-^
		//    2                ^--------^
		// 3		       ^-^
		//    3		                ^--------^
		// 4		         ^-^

		writer.Size(10);
		writer.Hop(5);
		writer[0] = 'h';
		writer[1] = 'e';
		writer[2] = 'l';
		writer[3] = 'l';
		writer[4] = 'o';
		writer.Produce();  // 1

		reader.Size(3);
		reader.Hop(2);

		reader.Consume();  // 1
		reader.Consume();  // 2
		CPPUNIT_ASSERT( false == reader.CanConsume() );

		writer[0] = ' ';
		writer[1] = 'w';
		writer[2] = 'o';
		writer[3] = 'r';
		writer[4] = 'l';
		writer[5] = 'd';
		writer.Produce(); // 2

		reader.Consume(); // 3
		reader.Consume(); // 4

		CPPUNIT_ASSERT( false == reader.CanConsume() );
		writer.Produce();  // without producing here, the following indexed acceses
		CPPUNIT_ASSERT_EQUAL( 'r', reader[0] );  // to the reader would fail an assert
		CPPUNIT_ASSERT_EQUAL( 'l', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'd', reader[2] );
	}

	void testStreamImplementation_avancingALongWay_semiStressTest()
	{
		const int lastTokenToConsume = 500;
		CLAM::WritingRegion<int,DataStructure> writer;
		typename CLAM::WritingRegion<int,DataStructure>::ProperReadingRegion reader;
		writer.LinkRegions( reader );
		for (int actual=0; actual < lastTokenToConsume; actual++)
		{
			writer[0] = actual;
			writer.Produce();

			CPPUNIT_ASSERT_EQUAL( actual, reader[0] );
			reader.Consume();
		}
	}

	// tests buffer-size

	// tests stl impl deletes old elements

	// tests varying regions sizes

	// tests inplace regions

	void testWritingRegion_CenterRegion_withOneReader()
	{

		//    	    012345678901
		//    W	    0abcdefgh
		//    R	    ------	 
		
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(4);
		writer.Hop(4);
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		reader.Size(6);
		reader.Hop(3);
		writer.LinkRegions( reader );
		writer.CenterEvenRegions();

		writer[0] = 'a';
		writer[1] = 'b';
		writer[2] = 'c';
		writer[3] = 'd';
		writer.Produce();
		writer[0] = 'e';
		writer[1] = 'f';
		writer[2] = 'g';
		writer[3] = 'h';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( '\0', reader[0] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader[1] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader[2] );
		CPPUNIT_ASSERT_EQUAL( 'a', reader[3] );
		CPPUNIT_ASSERT_EQUAL( 'b', reader[4] );
		CPPUNIT_ASSERT_EQUAL( 'c', reader[5] );
		
	}
	
	void testWritingRegion_CenterRegion_withOneReaderAndBiggerWriter()
	{

		//    	    012345678901
		//    W	    0abcdefgh
		//    R	    ------	 
		
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(4);
		writer.Hop(4);
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		reader.Size(2);
		reader.Hop(2);
		writer.LinkRegions( reader );
		writer.CenterEvenRegions();

		writer[0] = 'a';
		writer[1] = 'b';
		writer[2] = 'c';
		writer[3] = 'd';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( '\0', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'a', reader[1] );
	}

	void testWritingRegion_CenterRegion_assertsWithUnevenRegion()
	{

		//    	    012345678901
		//    W	    0abcdefgh
		//    R	    ------	 
		
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(4);
		writer.Hop(4);
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		reader.Size(5);
		reader.Hop(3);
		writer.LinkRegions( reader );
		try
		{
			writer.CenterEvenRegions();
			CPPUNIT_FAIL( "Exception should have been thrown");
		}
		catch( CLAM::ErrAssertionFailed & )
		{
		}
	}

	void testWritingRegion_CenterRegion_withSomeReaders()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(4);
		writer.Hop(4);
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader;
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader2;
		typename CLAM::WritingRegion<char,DataStructure>::ProperReadingRegion reader3;

		reader.Size(6);
		reader.Hop(3);
		reader2.Size(8);
		reader2.Hop(8);
		reader3.Size(2);
		reader3.Hop(2);
		
		writer.LinkRegions( reader );
		writer.LinkRegions( reader2 );
		writer.LinkRegions( reader3 );
		
		writer.CenterEvenRegions();

		writer[0] = 'a';
		writer[1] = 'b';
		writer[2] = 'c';
		writer[3] = 'd';
		writer.Produce();
		writer[0] = 'e';
		writer[1] = 'f';
		writer[2] = 'g';
		writer[3] = 'h';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( '\0', reader[0] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader[1] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader[2] );
		CPPUNIT_ASSERT_EQUAL( 'a', reader[3] );
		CPPUNIT_ASSERT_EQUAL( 'b', reader[4] );
		CPPUNIT_ASSERT_EQUAL( 'c', reader[5] );
	
		CPPUNIT_ASSERT_EQUAL( '\0', reader2[0] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader2[1] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader2[2] );
		CPPUNIT_ASSERT_EQUAL( '\0', reader2[3] );
		CPPUNIT_ASSERT_EQUAL( 'a', reader2[4] );
		CPPUNIT_ASSERT_EQUAL( 'b', reader2[5] );
		CPPUNIT_ASSERT_EQUAL( 'c', reader2[6] );
		CPPUNIT_ASSERT_EQUAL( 'd', reader2[7] );
		
		CPPUNIT_ASSERT_EQUAL( '\0', reader3[0] );
		CPPUNIT_ASSERT_EQUAL( 'a', reader3[1] );
	}

	// MRJ: This test passes on GNU C++ compiler since it
	// *does* initialize scalar types to zero.
	/*
	void testTDataConstructorInitializesToZero()
	{
		CLAM::TData zero( 0.0f );
		CLAM::TData defaultValue;
		double delta( 0.000001 );
				
		CPPUNIT_ASSERT_DOUBLES_EQUAL( zero, defaultValue, delta );
	}
	*/
	void testWritingRegion_GetLastWrittenData_withoutOverlap()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(2);
		writer.Hop(2);
		writer[0] = 'a';
		writer[1] = 'i';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( 'a', writer.GetLastWrittenData(0) );
		CPPUNIT_ASSERT_EQUAL( 'i', writer.GetLastWrittenData(1) );
	}
	
	void testWritingRegion_GetLastWrittenData_withOverlap()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(3);
		writer.Hop(3);
		writer.Produce();
		writer.Produce();
		writer[0] = 'g';
		writer[1] = 'h';
		writer[2] = 'i';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( 'g', writer.GetLastWrittenData(0) );
		CPPUNIT_ASSERT_EQUAL( 'h', writer.GetLastWrittenData(1) );
		CPPUNIT_ASSERT_EQUAL( 'i', writer.GetLastWrittenData(2) );

	}

	void testWritingRegion_GetLastWrittenData_withDifferentHopAndSize()
	{
		CLAM::WritingRegion<char,DataStructure> writer;
		writer.Size(2);
		writer.Hop(1);
		writer[0] = 'a';
		writer[1] = 'i';
		writer.Produce();
		
		CPPUNIT_ASSERT_EQUAL( 'a', writer.GetLastWrittenData(0) );
		CPPUNIT_ASSERT_EQUAL( 'i', writer.GetLastWrittenData(1) );
	
	}
	
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<CLAM::PhantomBuffer> );
//CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<std::vector> );
//CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<std::list> );
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<CLAM::StdVector> );
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStream<CLAM::StdList> );

} // namespace CLAMTest 

