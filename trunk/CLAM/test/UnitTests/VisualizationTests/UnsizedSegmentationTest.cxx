#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "UnsizedSegmentation.hxx"
#include "XMLTestHelper.hxx"
#include <sstream>

using CLAM::UnsizedSegmentation;
using CLAM::XmlStorage;

namespace CLAMTest
{
	class UnsizedSegmentationTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( UnsizedSegmentationTest );

	class UnsizedSegmentationTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( UnsizedSegmentationTest );
		CPPUNIT_TEST( testStoreOn);		
		CPPUNIT_TEST_SUITE_END();

	public:

		void testStoreOn()
		{
			const double onsets[]={90, 100, 110, 120};
			UnsizedSegmentation segmentation(200, onsets, onsets+4);
			std::ostringstream stream;
			XmlStorage::Dump(segmentation, "Segmentation", stream);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<Segmentation size=\"4\">90 100 110 120</Segmentation>"
				, stream.str());
		}
	};


}

