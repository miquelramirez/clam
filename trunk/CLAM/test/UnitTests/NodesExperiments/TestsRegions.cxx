#include <cppunit/extensions/HelperMacros.h>
#include "ReadingRegion.hxx"
#include "WritingRegion.hxx"

namespace CLAMTest {

class TestsRegions ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsRegions );

class TestsRegions : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsRegions );

	CPPUNIT_TEST( testWritingRegion_addRegion );
	CPPUNIT_TEST( testWritingRegion_removeRegion );
	CPPUNIT_TEST( testWritingRegion_removeRegion_withTwoReadingRegions );
	CPPUNIT_TEST( testWritingRegion_RemoveRegion_NotifiesReader );
	CPPUNIT_TEST_SUITE_END();

	void testWritingRegion_addRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.LinkRegions( reader );
		Region & baseReader = (Region &)reader;
		CPPUNIT_ASSERT_EQUAL( &baseReader, *(writer.BeginReaders()) );
	}
	void testWritingRegion_removeRegion()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;

		writer.LinkRegions( reader );
		writer.RemoveRegion( reader );
		CPPUNIT_ASSERT( writer.BeginReaders() == writer.EndReaders() );
	}
	void testWritingRegion_removeRegion_withTwoReadingRegions()
	{
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader1, reader2;
		writer.LinkRegions( reader1 );
		writer.LinkRegions( reader2 );
		writer.RemoveRegion( reader1 );

		Region & baseReader2 = (Region &)reader2;
		CPPUNIT_ASSERT_EQUAL( &baseReader2, *writer.BeginReaders() );
	}

	void testWritingRegion_RemoveRegion_NotifiesReader()
	{	
		WritingRegion<int> writer;
		WritingRegion<int>::ProperReadingRegion reader;	
		writer.LinkRegions( reader );
		CPPUNIT_ASSERT( &(writer) == reader.ProducerRegion() );

		writer.RemoveRegion( reader );

		CPPUNIT_ASSERT( 0 == reader.ProducerRegion() );
	}

};

} // namespace CLAMTest
