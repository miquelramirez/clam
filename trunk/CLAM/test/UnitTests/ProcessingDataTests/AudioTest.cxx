#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Audio.hxx" // CLAM
#include "Array.hxx" // CLAM

namespace CLAMTest
{

class AudioTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AudioTest );

class AudioTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AudioTest );

	CPPUNIT_TEST(testAudioInit_byDefault);
	CPPUNIT_TEST(testSetSize_reallocates);
	CPPUNIT_TEST(testResizeAudio_increasingSize);
	CPPUNIT_TEST(testResizeAudio_reducingSizeDoesNotReallocate);

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testAudioInit_byDefault()
	{
		CLAM::Audio prueba;
		CPPUNIT_ASSERT_EQUAL( 0, prueba.GetSize());
		CPPUNIT_ASSERT_EQUAL( 44100.0f, prueba.GetSampleRate() );
		CPPUNIT_ASSERT_EQUAL( 0., prueba.GetBeginTime() );
	}

	void testSetSize_reallocates()
	{
		CLAM::Audio prueba;
		prueba.SetSize(4);

		CPPUNIT_ASSERT_EQUAL( 4, prueba.GetSize() );
		CPPUNIT_ASSERT_EQUAL( 4, prueba.GetBuffer().Size() );
		CPPUNIT_ASSERT_EQUAL( 4, prueba.GetBuffer().AllocatedSize() );
	}

	void testResizeAudio_increasingSize()
 	{
		CLAM::Audio prueba;
		prueba.SetSize(4);
		prueba.SetSize(5);

		CPPUNIT_ASSERT_EQUAL( 5, prueba.GetSize() );
		CPPUNIT_ASSERT_EQUAL( 5, prueba.GetBuffer().Size() );
		CPPUNIT_ASSERT_EQUAL( 5, prueba.GetBuffer().AllocatedSize() );
 	}
	
 	void testResizeAudio_reducingSizeDoesNotReallocate()
 	{
		CLAM::Audio prueba;
		prueba.SetSize(4);
		prueba.SetSize(3);

		CPPUNIT_ASSERT_EQUAL( 3, prueba.GetSize());
		CPPUNIT_ASSERT_EQUAL( 3, prueba.GetBuffer().Size());
		CPPUNIT_ASSERT_EQUAL( 4, prueba.GetBuffer().AllocatedSize());
 	}

};

} // namespace CLAMTest

