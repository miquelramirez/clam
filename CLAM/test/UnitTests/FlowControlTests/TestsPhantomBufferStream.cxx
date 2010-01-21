
#include <cppunit/extensions/HelperMacros.h>
#include "PhantomBuffer.hxx" // CLAM
#include "WritingRegion.hxx" // CLAM
#include <list>
#include <iostream>

// TODO to remove. Just for debugging purposes
#include <cctype>
void Printbuffer(char* p, int size)
{
	std::cout << "\n\n\t0         1         2         3         4\n"
	"\t01234567890123456789012345678901234567890123456789\n\t";
	char notshowable = '·';
	for (int i=0; i<size; i++)
	{
		char foo = p[i];
		if (std::isalpha(foo) )
			std::cout << foo;
		else
			std::cout << notshowable;
	}
	std::cout << std::endl;
}
// end remove

namespace CLAMTest {

class TestsPhantomBufferStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsPhantomBufferStream );

/** Tests that not applies to other strema implementations */
class TestsPhantomBufferStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsPhantomBufferStream );
	
	CPPUNIT_TEST( testStreamDontIncreasesSizeAfterProducing );
	CPPUNIT_TEST( testStreamIncreasesSizeAfterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testPhantomBufferIncreasesAfterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testInsertionPositionInLogicalZone_afterLinkingWithBiggerRegion );
	CPPUNIT_TEST( testLogicalSizeIncreases_whenWriterIncreasesItsSize );
	CPPUNIT_TEST( testLogicalSizeRemainsTheSame_whenWriterDecreasesItsSize );
	CPPUNIT_TEST( testLogicalSizeIncreases_whenReaderIncreasesItsSize );
	CPPUNIT_TEST( testWriterRegionCantProduce_whenOverlapsReadingRegion );
	CPPUNIT_TEST( testWriting_rearmostPos_when2ReadingRegions );
	CPPUNIT_TEST( testWriter_sizeReservesSizePowOf2 );
  	CPPUNIT_TEST( testPhantomZoneGetsUpdated_whenWroteInBeginningZone );
	CPPUNIT_TEST( testBeginningZoneGetsUpdated_whenWroteInPhantomZone );
	CPPUNIT_TEST( testPhantomZoneGetsUpdated_whenInsertedInLogicalZone );
	CPPUNIT_TEST( testIntegrityAfterBufferResize );
	CPPUNIT_TEST( testWriterResizes_whenRearmostReaderHasSameBeginDistance );
	CPPUNIT_TEST( testReadingRegion_canReadInPhantomZone );
	CPPUNIT_TEST_SUITE_END();
	/*
	 *
	 *   Concrete Phantom Buffer tests (checking phantom zone, overlapping, etc.
	 *
	 */
	void testStreamDontIncreasesSizeAfterProducing()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(5);
		writer.Hop(2);
		int initialLogicalSize = writer.LogicalStreamSize();
		writer.Produce();
		CPPUNIT_ASSERT_EQUAL(initialLogicalSize, writer.LogicalStreamSize() );
	}


	//tests of logicalSize changed when link with a bigger reading region
	void testStreamIncreasesSizeAfterLinkingWithBiggerRegion()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);
		int sizeWithALonelyWritingRegion = writer.LogicalStreamSize();

		reader.Size(9);
		reader.Hop(2);
		writer.LinkRegions(reader);

		CPPUNIT_ASSERT( sizeWithALonelyWritingRegion < writer.LogicalStreamSize() );
	}

	// test phantom buffer increases after linking with bigger reading region
	void testPhantomBufferIncreasesAfterLinkingWithBiggerRegion()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);

		reader.Size(9);
		reader.Hop(2);
		int oldPhantomSize = writer.Stream().PhantomSize();
		writer.LinkRegions(reader);

		CPPUNIT_ASSERT( oldPhantomSize < writer.Stream().PhantomSize() );
	}

	// test resize reading region which is not in the begining
	// here we want to test the insertion point.
	void testInsertionPositionInLogicalZone_afterLinkingWithBiggerRegion()
	{
	CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);

		writer[0] = 'g';
		writer[1] = 'o';
		writer[2] = 'o';
		writer[3] = 'd';
		writer[4] = '\0';
		writer.Produce();

		reader.Size(9);
		reader.Hop(2);

  		writer.LinkRegions(reader);

		CPPUNIT_ASSERT_EQUAL( std::string("od"), 
				std::string( &(writer.Stream().operator[](18))) ); 
		//we are inserting 16 elems + offset of 2
	}

	void testLogicalSizeIncreases_whenWriterIncreasesItsSize()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);
		int initialSize = writer.LogicalStreamSize();
		writer.Size(10);
		CPPUNIT_ASSERT(initialSize < writer.LogicalStreamSize() );
	}

	void testLogicalSizeRemainsTheSame_whenWriterDecreasesItsSize()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);
		int initialSize = writer.LogicalStreamSize();
		writer.Size(3);
		CPPUNIT_ASSERT(initialSize == writer.LogicalStreamSize() );
	}

	//tests of logicalSize changed when reader changes its size
	void testLogicalSizeIncreases_whenReaderIncreasesItsSize()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(5);
		writer.Hop(2);

		reader.Size(2);
		reader.Hop(2);
		int initialSize = writer.LogicalStreamSize();
		writer.LinkRegions(reader);

		CPPUNIT_ASSERT( initialSize == writer.LogicalStreamSize() );
		reader.Size( 9 );

		CPPUNIT_ASSERT( initialSize < writer.LogicalStreamSize() );
	}

	//tests of !canProduce (circular overlap)
	void testWriterRegionCantProduce_whenOverlapsReadingRegion()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;

		writer.Size(4);
		writer.Hop(1); // so we can position it at the conflictive point
		reader.Size(1);
		writer.LinkRegions(reader);
		while( writer.Pos() <= writer.LogicalStreamSize() - writer.Size() )
		{
			writer[0] = 'X';
			writer.Produce();
		}
		CPPUNIT_ASSERT( writer.CanProduce() == false );
	}

	void testWriting_rearmostPos_when2ReadingRegions()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion firstReader;
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion lastReader;
		writer.Size(6);
		writer.Hop(6);
		firstReader.Size(4);
		firstReader.Hop(4);
		lastReader.Size(6);
		lastReader.Hop(3);
		writer.LinkRegions(firstReader);
		writer.LinkRegions(lastReader);
		writer.Produce();
		firstReader.Consume();
		lastReader.Consume();
		CPPUNIT_ASSERT_EQUAL( 3, writer.RearmostReadingPos() );
	}

	void testWriter_sizeReservesSizePowOf2()
	{
		int powOfNine = 512; // 512 == 2^9  (2^8 == 256)

		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size( 258 / 2 ); // logical size will be the power of 2 greater and closer to size*2
		CPPUNIT_ASSERT_EQUAL( powOfNine, writer.Stream().LogicalSize() );
	}

	void testPhantomZoneGetsUpdated_whenWroteInBeginningZone()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(5); // logical size == 16  (tbe pow of 2 > 5*2)
		writer.Hop(5);
		//  food          |phan|
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^---^
		//2      ^---^

		writer[0] = 'd';
		writer[1] = 'o';
		writer[2] = 'g';
		writer[3] = '\0';
		// it IS necessary to do a writer.Produce() for updating phantom zone
		writer.Produce();

		char *bufferbase = &(writer.Stream().operator[](0) );
		//printbuffer(bufferbase, 22);

		CPPUNIT_ASSERT_EQUAL( std::string("dog"), std::string(bufferbase+16) );

	}

	// idem but accessing by the pointer
	// TODO

	void testBeginningZoneGetsUpdated_whenWroteInPhantomZone()
	{	

		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(6); // logical size == 16  (tbe pow of 2 > 6*2)
		writer.Hop(6);
		//             goodbye
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^----^         [phn]
		//2       ^----^
		//3             ^----^
		char* bufferbase = &(writer.Stream().operator[](0));
		
		writer.Produce(); // now writer.pos == 6
		writer.Produce(); // now writer.pos == 12
		char* toWrite = &writer[0];
		toWrite[0] = 'g';  // buffer[12]
		toWrite[1] = 'o';  // buffer[13]
		toWrite[2] = 'o';  // buffer[14]
		toWrite[3] = 'd';  // buffer[15]
		toWrite[4] = 'b';  // buffer[16] -- first position of phantom buffer
		toWrite[5] = 'y';
		writer.Produce();
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;
		CPPUNIT_ASSERT_EQUAL('b', bufferbase[0] );
		CPPUNIT_ASSERT_EQUAL('y', bufferbase[1] );
	}

	void testPhantomZoneGetsUpdated_whenInsertedInLogicalZone()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(5); // logical size == 16  (tbe pow of 2 > 5*2)
		writer.Hop(5);
		//  goodbye
		//  0        1        2        3         4
		//  0·······90·······90·······90········90
		//1 ^---^                        [phantm]
		//2      ^---^
		//3      ^-------^
		writer[0] = 'g';
		writer[1] = 'o';
		writer[2] = 'o';
		writer[3] = 'd';
		writer[4] = 'b';
		writer.Produce();
		writer[0] = 'y'; // writer.pos == 5
		writer[1] = 'e'; // writer.pos == 6
		writer[2] = ' '; // writer.pos == 7
		writer[3] = 'h';
		writer[4] = 'i';
		//printbuffer( &(writer.Stream().operator[](0)), 32 );
		// the following resize inserts at writer.pos == 5
		writer.Size(9); // logical size == 32 (the pow of 2 > 9*2)

		//printbuffer( &(writer.Stream().operator[](0)), 32+5 );

		char* bufferbase = &(writer.Stream().operator[](0));
		CPPUNIT_ASSERT_EQUAL('y', bufferbase[21] );
		CPPUNIT_ASSERT_EQUAL('e', bufferbase[22] );

		CPPUNIT_ASSERT_EQUAL('g', bufferbase[32] );
		CPPUNIT_ASSERT_EQUAL('o', bufferbase[33] );
		CPPUNIT_ASSERT_EQUAL('o', bufferbase[34] );
		CPPUNIT_ASSERT_EQUAL('d', bufferbase[35] );
		//CPPUNIT_ASSERT_EQUAL('b', bufferbase[36] );
		
		// TODO: when we want enlarge the logical-size (using size() )
		// the phantom-buffer do not update using the new space.
	}
	void testIntegrityAfterBufferResize()
	{
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(2); // buff size == 4
		writer.Hop(2);
		writer[0]='A';
		writer[1]='B';
		writer.Produce();
		writer[0]='C';
		writer[1]='D';
		writer.Produce();

		writer.Size(4); // buff size == 8
		writer.Hop(4);
		writer[0]='X';
		writer[1]='X';
		writer[2]='X';
		writer[3]='X';
		writer.Produce();
		CPPUNIT_ASSERT_EQUAL('A', writer[0]);
	}
	
	void testWriterResizes_whenRearmostReaderHasSameBeginDistance()
	{	
		CLAM::WritingRegion<char, CLAM::PhantomBuffer> writer;
		writer.Size(3); // buff size = 8
		writer.Hop(3);
	
		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;
		writer.LinkRegions(reader);
		reader.Size(1);
		reader.Hop(1);

		writer.Produce(); // pos = 3, beginDistance = 3
		writer.Produce(); // pos = 6, beginDistance = 6
		reader.Consume(); // pos = 1, beginDistance = 1
		writer.Produce(); // pos = 9, beginDistance = 1
		int oldBeginDistance = reader.BeginDistance();
		
		writer.Size(5);   // buff size = 16
		CPPUNIT_ASSERT( oldBeginDistance != reader.BeginDistance() );
	}

	void testReadingRegion_canReadInPhantomZone()
	{
		CLAM::WritingRegion<char,CLAM::PhantomBuffer> writer;
		writer.Size(3);
		writer.Hop(3);

		CLAM::WritingRegion<char, CLAM::PhantomBuffer>::ProperReadingRegion reader;
		writer.LinkRegions(reader);
		reader.Size(3);
		reader.Hop(3);

		writer.Produce();
		reader.Consume();
		
		writer.Produce();
		reader.Consume();
		
		writer[0] = 'g';
		writer[1] = 'h';
		writer[2] = 'i';
		writer.Produce();

		CPPUNIT_ASSERT_EQUAL( 'g', reader[0] );
		CPPUNIT_ASSERT_EQUAL( 'h', reader[1] );
		CPPUNIT_ASSERT_EQUAL( 'i', reader[2] );
	}
};






} // namespace CLAMTest 

