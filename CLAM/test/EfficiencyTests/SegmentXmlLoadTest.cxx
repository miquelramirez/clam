
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"	
#include "EfficiencyTest.hxx"

#include "Segment.hxx"
#include "XMLStorage.hxx"
#include <string>
#include <fstream>

namespace CLAMTest
{

class SegmentXmlLoadEfficiencyTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SegmentXmlLoadEfficiencyTest );

class SegmentXmlLoadEfficiencyTest : public EfficiencyTest
{
public:
	
	CPPUNIT_TEST_SUITE( SegmentXmlLoadEfficiencyTest );
	
//	CPPUNIT_TEST( testPathToFiles );
	CPPUNIT_TEST( testLoadSegment);
	
	CPPUNIT_TEST_SUITE_END();

	std::string mPathToTestData;


public: // TestFixture interface

	void setUp()
	{
		mPathToTestData = GetTestDataDirectory();
	}
	void tearDown()
	{
	}

private:
/*
	void testPathToFiles()
	{
		std::string msg = "Cound't open this file: " + mPathToTestData+"sine.wav";
		CPPUNIT_ASSERT_MESSAGE(msg, helperFileExist(mPathToTestData+"sine.wav"));
	}
*/
	void testLoadSegment()
	{
		start();
		CLAM::Segment inputSegment;
		CLAM::XMLStorage::Restore( inputSegment, mPathToTestData+"/SMSAnalysisTests/sweep_segment.xml" );
		stop();
	}

};

} // namespace CLAMTest

