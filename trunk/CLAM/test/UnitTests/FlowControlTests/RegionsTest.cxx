#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "StreamBuffer.hxx"
#include "ReadStreamRegion.hxx"
#include "WriteStreamRegion.hxx"
#include "StreamRegionContainer.hxx"
#include "Audio.hxx"
#include "CircularStreamImpl.hxx"

namespace CLAMTest
{


class RegionsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( RegionsTest );

class RegionsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( RegionsTest );
	CPPUNIT_TEST( testReadStreamRegion_CanActivate_WhenWriterIsJustInitialized );
	CPPUNIT_TEST( testWriteStreamRegion_CanActivate_WhenWriterIsJustInitialized );
	CPPUNIT_TEST( testWriteStreamRegion_CanActivate_WhenWriterIsOutOfBufferSpace );
	// todo: test ReadSR and WriteSR CanActivate when not in initial state and activating implies overlap.
	CPPUNIT_TEST( testStreamRegionContainer_RemoveReader_WithoutReadersThrowsException );
	CPPUNIT_TEST( testStreamRegionContainer_RemoveReader_WithoutCorrectReaderThrowsException );
	CPPUNIT_TEST( testStreamRegionContainer_RemoveReader_WithCorrectReader );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testReadStreamRegion_CanActivate_WhenWriterIsJustInitialized()
	{
		// Setup
		const int hop = 1;
		const int length = 1;
		CLAM::StreamBuffer<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > buffer;
		// Exercise
		CLAM::WriteStreamRegion * write = buffer.NewWriter( hop, length );
		CLAM::ReadStreamRegion * read = buffer.NewReader( hop, length );

		// Verification
		CPPUNIT_ASSERT_EQUAL_MESSAGE("shouldn't be able to activate because reader overlaps writer", 
			false, read->CanActivate() );
		
		// Tear down
		delete write;
		delete read;
	}
	
	void testWriteStreamRegion_CanActivate_WhenWriterIsJustInitialized()
	{
		// Setup
		const int hop = 1;
		const int length = 1;
		CLAM::StreamBuffer<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > buffer;
		// Exercise
		CLAM::WriteStreamRegion * write = buffer.NewWriter( hop, length );
		CLAM::ReadStreamRegion * read = buffer.NewReader( hop, length );

		// Verification
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"the region always let writer to activate. It doesn't know about buffering implementation",
			true, write->CanActivate() );
		
		// Tear down
		delete write;
		delete read;
	}

	void testWriteStreamRegion_CanActivate_WhenWriterIsOutOfBufferSpace()
	{
		// Setup
		const int hop = 1;
		const int length = 1;
		CLAM::StreamBuffer<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > buffer;
		// Exercise
		CLAM::WriteStreamRegion * write = buffer.NewWriter( hop, length );
		CLAM::ReadStreamRegion * read = buffer.NewReader( hop, length );

		// Verification
		write->Activate();
		write->LeaveAndAdvance(); 
		// This is a tricky test: notice buffer length is 1. region should be out of buffer space.
		// but Region::CanActivate() only have to check reading-over-source region overlapping.
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"the region always lets writer to activate. It doesn't know about buffering implementation restrictions",
			true, write->CanActivate() );
		
		// Tear down
		delete write;
		delete read;
	}

	void testStreamRegionContainer_RemoveReader_WithoutReadersThrowsException ()
	{
		const int hop = 1;
		const int length = 1;
		CLAM::StreamRegionContainer container;
		CLAM::WriteStreamRegion * writer = 
			new CLAM::WriteStreamRegion( hop, length );
		CLAM::ReadStreamRegion * reader = 
			new CLAM::ReadStreamRegion( hop, length, writer );
		container.SetWriter( writer );
		try{
			container.RemoveReader( reader );
			CPPUNIT_FAIL("Assert expected but nothing happened");
		} catch ( CLAM::ErrAssertionFailed& ) {}

		delete writer;
		delete reader;
	}

	void testStreamRegionContainer_RemoveReader_WithoutCorrectReaderThrowsException ()
	{
		const int hop = 1;
		const int length = 1;
		CLAM::StreamRegionContainer container;
		CLAM::WriteStreamRegion * writer = new CLAM::WriteStreamRegion( hop, length );
		CLAM::ReadStreamRegion * reader = new CLAM::ReadStreamRegion( hop, length, writer );
		CLAM::ReadStreamRegion * reader2 = new CLAM::ReadStreamRegion( hop, length, writer );
		container.SetWriter( writer );
		container.AddReader( reader );
		try{
			container.RemoveReader( reader2 );
			CPPUNIT_FAIL("Assert expected but nothing happened");
		} catch ( CLAM::ErrAssertionFailed& ) {}
		
		delete writer;
		delete reader;
		delete reader2;

	}

	void testStreamRegionContainer_RemoveReader_WithCorrectReader()
	{
		const int hop = 1;
		const int length = 1;
		CLAM::StreamRegionContainer container;
		CLAM::WriteStreamRegion * writer = new CLAM::WriteStreamRegion( hop, length );
		CLAM::ReadStreamRegion * reader = new CLAM::ReadStreamRegion( hop, length, writer );
		container.SetWriter( writer );
		container.AddReader( reader );
		container.RemoveReader( reader );
		
		CPPUNIT_ASSERT_EQUAL( false, writer->IsSourceOf( reader ));

		delete writer;
		delete reader;
	}

};






} // namespace CLAMTest
