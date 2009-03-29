#ifdef USE_XMLPP

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "LibXmlDomWriter.hxx" // CLAM
#include <libxml++/libxml++.h>

namespace CLAM
{
namespace Test
{

class LibXmlDomWriterTest;

CPPUNIT_TEST_SUITE_REGISTRATION( LibXmlDomWriterTest );

class LibXmlDomWriterTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( LibXmlDomWriterTest );
	CPPUNIT_TEST(testEmptyDocument);
	CPPUNIT_TEST(testPlainContent);
	CPPUNIT_TEST(testEmptyElement);
	CPPUNIT_TEST(testElementWithPlainContent);
	CPPUNIT_TEST(testElementWithInnerElement);
	CPPUNIT_TEST(testEmptyElementWithAttribute);
	CPPUNIT_TEST(testEmptyElementWithAttributes);
	CPPUNIT_TEST(testEmptyElementWithReversedOrderAttributes);
	CPPUNIT_TEST(testEmptyElementWithSameAttributeNameTwice);
	CPPUNIT_TEST(testNonEmptyElementWithAttribute);
	CPPUNIT_TEST(testDocumentWithFullContent);
	CPPUNIT_TEST(testDocumentWithFullContentAndIndentation);
	CPPUNIT_TEST(testDocumentWithFullContentAndIndentation_elementWithContentAreInlined);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		
		mTargetStream.str("");
		mDocument = new xmlpp::Document;
		mRoot = mDocument->create_root_node(
			"TestDoc" // root element name
//			,"http://iua.upf.es/mtg/clam/0.8.0/2005-06" // root element namespace URI.
//			,"clam" // namespace prefix
		);
		mHeader = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
		delete mDocument;
	}

private:

	std::stringstream mTargetStream;
	xmlpp::Document * mDocument;
	xmlpp::Element * mRoot;
	std::string mHeader;
	void assertXmlBodyEqual(const std::string & body)
	{
		CPPUNIT_ASSERT_EQUAL(mHeader + body +"\n", mTargetStream.str());
	}

	void testEmptyDocument()
	{
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc/>");
	}
	void testPlainContent()
	{
		mRoot->add_child_text("Content");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc>Content</TestDoc>");
	}
	void testEmptyElement()
	{
		mRoot->add_child("EmptyElement");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><EmptyElement/></TestDoc>");
	}
	void testElementWithPlainContent()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->add_child_text("Content");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><Element>Content</Element></TestDoc>");
	}
	void testElementWithInnerElement()
	{
		mRoot
			->add_child("Element")
			->add_child("InnerElement")
			->add_child_text("Content");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><Element><InnerElement>Content</InnerElement></Element></TestDoc>");
	}
	void testEmptyElementWithAttribute()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->set_attribute("attribute","Attribute value");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><Element attribute=\"Attribute value\"/></TestDoc>");
	}
	void testEmptyElementWithAttributes()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->set_attribute("attribute1","Attribute 1 value");
		domElement->set_attribute("attribute2","Attribute 2 value");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><Element attribute1=\"Attribute 1 value\" attribute2=\"Attribute 2 value\"/></TestDoc>");
	}
	void testEmptyElementWithReversedOrderAttributes()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->set_attribute("attribute2","Attribute 2 value");
		domElement->set_attribute("attribute1","Attribute 1 value");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		// Unlike Xerces-C it outputs attributes in insertion order
		assertXmlBodyEqual("<TestDoc><Element attribute2=\"Attribute 2 value\" attribute1=\"Attribute 1 value\"/></TestDoc>");
	}
	void testEmptyElementWithSameAttributeNameTwice()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->set_attribute("attribute","Attribute value 1");
		domElement->set_attribute("attribute","Attribute value 2");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		// It gets the last one
		assertXmlBodyEqual("<TestDoc><Element attribute=\"Attribute value 2\"/></TestDoc>");
	}
	void testNonEmptyElementWithAttribute()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->add_child("InnerElement");
		domElement->set_attribute("attribute","Attribute value");
		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual("<TestDoc><Element attribute=\"Attribute value\"><InnerElement/></Element></TestDoc>");
	}
	void testDocumentWithFullContent()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->add_child_text("Content");
		domElement->add_child("InnerElement");
		domElement->set_attribute("attribute","Attribute value");

		LibXmlDomWriter writer;
		writer.write(mTargetStream, mDocument);

		assertXmlBodyEqual(
			"<TestDoc>"
				"<Element attribute=\"Attribute value\">"
					"Content"
					"<InnerElement/>"
				"</Element>"
			"</TestDoc>"
			);
	}
	void testDocumentWithFullContentAndIndentation()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		domElement->add_child("InnerElement")->add_child_text("Content");
		domElement->set_attribute("attribute","Attribute value");

		LibXmlDomWriter writer;
		writer.DoIndentedFormat(true);
		writer.write(mTargetStream, mDocument);

		// LibXml does a very weird indentation!!

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<TestDoc>\n"
			"  <Element attribute=\"Attribute value\">\n"
			"    <InnerElement>Content</InnerElement>\n"
			"  </Element>\n"
			"</TestDoc>\n"
			), mTargetStream.str());
	}
	void testDocumentWithFullContentAndIndentation_elementWithContentAreInlined()
	{
		xmlpp::Element * domElement = mRoot->add_child("Element");
		xmlpp::Element * subElement = domElement->add_child("InnerElement");
		subElement->add_child_text("Content");
		subElement->add_child("InnestElement");
		domElement->set_attribute("attribute","Attribute value");

		LibXmlDomWriter writer;
		writer.DoIndentedFormat(true);
		writer.write(mTargetStream, mDocument);

		// LibXml does a very weird indentation!!

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<TestDoc>\n"
			"  <Element attribute=\"Attribute value\">\n"
			"    <InnerElement>Content<InnestElement/></InnerElement>\n"
			"  </Element>\n"
			"</TestDoc>\n"
			), mTargetStream.str());
	}
};




} // namespace Test
} // namespace CLAM

#endif//USE_XMLPP
