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
		CPPUNIT_TEST( testLoadFromWithoutMaxPos);
		CPPUNIT_TEST( testLoadFromWithMaxPos);
		CPPUNIT_TEST(testStoreOn_withinAnArray);
		CPPUNIT_TEST(testLoadFromWithoutMaxPos_withinAnArray);
		CPPUNIT_TEST(testLoadFromWithMaxPos_withinAnArray);
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
		void testLoadFromWithoutMaxPos()
		{
			UnsizedSegmentation segmentation(200);
			std::istringstream stream("<Segmentation size=\"4\">90 100 110 120</Segmentation>");
			XmlStorage::Restore(segmentation, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentation.boundsAsString());
		}
		void testLoadFromWithMaxPos()
		{
			UnsizedSegmentation segmentation;
			std::istringstream stream("<Segmentation max=\"200\" size=\"4\">90 100 110 120</Segmentation>");
			XmlStorage::Restore(segmentation, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentation.boundsAsString());
		}

		void testStoreOn_withinAnArray()
		{
			const TData onsets1[]={90, 100, 110, 120};
			const TData onsets2[]={9, 10, 11};
			CLAM::Array<UnsizedSegmentation> segmentations(2);
			segmentations[0].maxPosition(200);
			segmentations[0].takeArray(onsets1, onsets1+4);
			segmentations[1].maxPosition(20);
			segmentations[1].takeArray(onsets2, onsets2+3);
			std::ostringstream stream;
			XmlStorage::Dump(segmentations, "Segmentations", stream);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<Segmentations>"
					"<UnsizedSegmentation max=\"200\" size=\"4\">90 100 110 120</UnsizedSegmentation>"
					"<UnsizedSegmentation max=\"20\" size=\"3\">9 10 11</UnsizedSegmentation>"
				"</Segmentations>"
				, stream.str());
		}
		void testLoadFromWithoutMaxPos_withinAnArray()
		{
			CLAM::Array<UnsizedSegmentation> segmentations;
			std::istringstream stream(
				"<Segmentations>"
					"<UnsizedSegmentation size=\"4\">90 100 110 120</UnsizedSegmentation>"
					"<UnsizedSegmentation size=\"3\">9 10 11</UnsizedSegmentation>"
				"</Segmentations>");
			XmlStorage::Restore(segmentations, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentations[0].boundsAsString());
			CPPUNIT_ASSERT_EQUAL(std::string("(9,9) (10,10) (11,11) "), segmentations[1].boundsAsString());
		}
		void testLoadFromWithMaxPos_withinAnArray()
		{
			CLAM::Array<UnsizedSegmentation> segmentations;
			std::istringstream stream(
				"<Segmentations>"
					"<UnsizedSegmentation max=\"200\" size=\"4\">90 100 110 120</UnsizedSegmentation>"
					"<UnsizedSegmentation max=\"20\" size=\"3\">9 10 11</UnsizedSegmentation>"
				"</Segmentations>");
			XmlStorage::Restore(segmentations, stream);
			CPPUNIT_ASSERT_EQUAL(std::string("(90,90) (100,100) (110,110) (120,120) "), segmentations[0].boundsAsString());
			CPPUNIT_ASSERT_EQUAL(std::string("(9,9) (10,10) (11,11) "), segmentations[1].boundsAsString());
		}
	};


}

