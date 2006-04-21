
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "ChordExtractor.hxx"
#include <iosfwd>

namespace Simac
{

namespace Test
{

class ChordExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ChordExtractorTest );

class ChordExtractorTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ChordExtractorTest );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:
	void testSilence()
	{
		ChordExtractor estimator;
		std::vector<float> inputData(30000);
		estimator.doIt(&inputData[0]);
		CPPUNIT_ASSERT_EQUAL(std::string("None"), estimator.chordEstimation());
	}
};




} // namespace Test
} // namespace Simac

