#include <cppunit/extensions/HelperMacros.h>

#include "WritingRegion.hxx" // CLAM
#include "ReadingRegion.hxx" // CLAM
#include <list>

/**
 * Into this test suite goes all specific tests that only aply to STL based
 * stream and not to phantom-buffer stream
 */
 //TODO it should be a template (stl container) class like TestsStream
namespace CLAMTest {

class TestsStlBasedStream ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsStlBasedStream );

class TestsStlBasedStream : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsStlBasedStream );

	CPPUNIT_TEST( testStreamIncreasesSizeAfterProducing );

	CPPUNIT_TEST_SUITE_END();

	// tests of size of stream
	void testStreamIncreasesSizeAfterProducing()
	{
		CLAM::WritingRegion<char,CLAM::StdList> writer;
		writer.Size(5);
		writer.Hop(2);
		CPPUNIT_ASSERT_EQUAL(5, writer.LogicalStreamSize() );
		writer.Produce();
		CPPUNIT_ASSERT_EQUAL(7, writer.LogicalStreamSize() );
	}

};

} // namespace CLAMTest 

