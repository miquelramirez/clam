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
		CPPUNIT_TEST( testtakeArray);
		CPPUNIT_TEST( testLoadFrom);
		CPPUNIT_TEST_SUITE_END();

	public:

		void testStoreOn()
		{
			const TData onsets[]={90, 100, 110, 120};
			UnsizedSegmentation segmentation(200, onsets, onsets+4);
			std::ostringstream stream;
			XmlStorage::Dump(segmentation, "Segmentation", stream);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<Segmentation max=\"200\" size=\"4\">90 100 110 120</Segmentation>"
				, stream.str());
		}

		void testtakeArray()
		{
			const TData bounds[]={90, 100, 110, 120};
			UnsizedSegmentation segmentation(200);
			segmentation.takeArray(bounds, bounds+4);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentation.boundsAsString());
		}
		void testLoadFrom()
		{
			UnsizedSegmentation segmentation(200);
			std::istringstream stream("<Segmentation size=\"4\">90 100 110 120</Segmentation>");
			XmlStorage::Restore(segmentation, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentation.boundsAsString());
		}
	};


}

