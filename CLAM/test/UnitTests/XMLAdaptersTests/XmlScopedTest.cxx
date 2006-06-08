#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <string>

#include "XmlScoped.hxx" // CLAM

namespace CLAM
{
namespace Test
{

class XmlScopedTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XmlScopedTest );

class XmlScopedTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XmlScopedTest );
	CPPUNIT_TEST( testInsertInteger );
	CPPUNIT_TEST( testInsertString );
	CPPUNIT_TEST( testEmptyElement );
	CPPUNIT_TEST( testElementWithContent );
	CPPUNIT_TEST( testEmptyElementWithAttribute );
	CPPUNIT_TEST( testEmptyElementWithFloatAttribute );
	CPPUNIT_TEST( testElementWithAttributeAndContent );
	CPPUNIT_TEST( testElementWithAttributes );
	CPPUNIT_TEST( testElementWithChildren );
//	CPPUNIT_TEST( test );
//	CPPUNIT_TEST( test );
//	CPPUNIT_TEST( test );
//	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		mTargetStream.str("");
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:

	std::stringstream mTargetStream;

	void testInsertInteger()
	{
		XmlFragment theContext(mTargetStream);
		XmlContent content(3);
		CPPUNIT_ASSERT_EQUAL(std::string("3"), mTargetStream.str());
	}

	void testInsertString()
	{
		XmlFragment theContext(mTargetStream);
		XmlContent content("A dummy text");
		CPPUNIT_ASSERT_EQUAL(std::string("A dummy text"), mTargetStream.str());
	}

	void testEmptyElement()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
		}
		CPPUNIT_ASSERT_EQUAL(std::string("<Tag1 />"), mTargetStream.str());
	}

	void testElementWithContent()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
			XmlContent content("A dummy text");
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1>A dummy text</Tag1>"), 
			mTargetStream.str());
	}
	
	void testEmptyElementWithAttribute()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
			XmlAttribute a("attrib11","value11");
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1 attrib11='value11' />"), 
			mTargetStream.str());
	}

	void testEmptyElementWithFloatAttribute()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
			XmlAttribute a("attrib11",2.34343);
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1 attrib11='2.34343' />"), 
			mTargetStream.str());
	}

	void testElementWithAttributeAndContent()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
			XmlAttribute a("attrib11","value11");
			XmlContent content("A dummy text");
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1 attrib11='value11'>A dummy text</Tag1>"), 
			mTargetStream.str());
	}

	void testElementWithAttributes()
	{
		XmlFragment theContext(mTargetStream);
		{ XmlElement tag1("Tag1");
			XmlAttribute a1("attrib11","value11");
			XmlAttribute a2("attrib12","value12");
			XmlContent content("A dummy text");
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1 attrib11='value11' attrib12='value12'>"
				"A dummy text</Tag1>"), 
			mTargetStream.str());
	}

	void testElementWithChildren()
	{
		XmlFragment theContext(mTargetStream);
		{ 
			XmlElement tag1("Tag1");
			{ 
				XmlElement tag11("Tag11");
				XmlContent content("A dummy text");
			}
			{
				XmlElement tag12("Tag12");
			}
			
		}
		CPPUNIT_ASSERT_EQUAL(
			std::string("<Tag1><Tag11>A dummy text</Tag11><Tag12 /></Tag1>"), 
			mTargetStream.str());
	}

};




} // namespace Test
} // namespace CLAM

