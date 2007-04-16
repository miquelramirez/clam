#ifdef USE_XERCES_TRUE

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XercesDomWriter.hxx" // CLAM
#include "XercesInitializer.hxx" // CLAM
#include "XercesEncodings.hxx" // CLAM
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

namespace CLAM
{
namespace Test
{

class XercesDomWriterTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomWriterTest );

class XercesDomWriterTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomWriterTest );
	CPPUNIT_TEST(testPlainContent);
	CPPUNIT_TEST(testEmptyElement);
	CPPUNIT_TEST(testElementWithPlainContent);
	CPPUNIT_TEST(testElementWithInnerElement);
	CPPUNIT_TEST(testEmptyElementWithAttribute);
	CPPUNIT_TEST(testEmptyElementWithAttributes);
	CPPUNIT_TEST(testEmptyElementWithReversedOrderAttributes);
	CPPUNIT_TEST(testNonEmptyElementWithAttribute);
	CPPUNIT_TEST(testEmptyDocument);
	CPPUNIT_TEST(testDocumentWithFullContent);
	CPPUNIT_TEST(testDocumentWithFullContentAndIndentation);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		XercesInitializer::require();
		mTargetStream.str("");
		xercesc::DOMImplementation * imp = 
			xercesc::DOMImplementation::getImplementation();
		mDocument = imp->createDocument(
			X("2003-04.clam05.iua.mtg.upf.es"), // root element namespace URI.
			X("TestDoc"), // root element name
			0  // document type object (DTD).
		);
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
		mDocument->release();
	}

private:

	std::stringstream mTargetStream;
	xercesc::DOMDocument * mDocument;

	void testPlainContent()
	{
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		XercesDomWriter writer;
		writer.write(mTargetStream, domContent);

		CPPUNIT_ASSERT_EQUAL(std::string("Content"), mTargetStream.str());
		domContent->release();

	}

	void testEmptyElement()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("EmptyElement"));
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<EmptyElement/>"), mTargetStream.str());

	}

	void testElementWithPlainContent()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		domElement->appendChild(domContent);
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element>Content</Element>"), mTargetStream.str());

	}

	void testElementWithInnerElement()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * domInnerElement = mDocument->createElement(X("InnerElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		domInnerElement->appendChild(domContent);
		domElement->appendChild(domInnerElement);
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element><InnerElement>Content</InnerElement></Element>"), mTargetStream.str());

	}

	void testEmptyElementWithAttribute()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		domElement->setAttribute(X("attribute"),X("Attribute value"));
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element attribute=\"Attribute value\"/>"), mTargetStream.str());

	}

	void testEmptyElementWithAttributes()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		domElement->setAttribute(X("attribute1"),X("Attribute 1 value"));
		domElement->setAttribute(X("attribute2"),X("Attribute 2 value"));
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element attribute1=\"Attribute 1 value\" attribute2=\"Attribute 2 value\"/>"), mTargetStream.str());

	}

	void testEmptyElementWithReversedOrderAttributes()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		domElement->setAttribute(X("attribute2"),X("Attribute 2 value"));
		domElement->setAttribute(X("attribute1"),X("Attribute 1 value"));
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element attribute1=\"Attribute 1 value\" attribute2=\"Attribute 2 value\"/>"), mTargetStream.str());

	}

	void testNonEmptyElementWithAttribute()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * domInnerElement = mDocument->createElement(X("InnerElement"));
		domElement->setAttribute(X("attribute"),X("Attribute value"));
		domElement->appendChild(domInnerElement);
		XercesDomWriter writer;
		writer.write(mTargetStream, domElement);

		CPPUNIT_ASSERT_EQUAL(std::string("<Element attribute=\"Attribute value\"><InnerElement/></Element>"), mTargetStream.str());
	}
	
	void testEmptyDocument()
	{
		XercesDomWriter writer;
		writer.write(mTargetStream, mDocument);

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
			"<TestDoc xmlns=\"2003-04.clam05.iua.mtg.upf.es\"/>"		
			), mTargetStream.str());
	}

	void testDocumentWithFullContent()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * domInnerElement = mDocument->createElement(X("InnerElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		domElement->setAttribute(X("attribute"),X("Attribute value"));
		domElement->appendChild(domContent);
		domElement->appendChild(domInnerElement);
		mDocument->getDocumentElement()->appendChild(domElement);
		
		XercesDomWriter writer;
		writer.write(mTargetStream, mDocument);

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
			"<TestDoc xmlns=\"2003-04.clam05.iua.mtg.upf.es\">"
				"<Element attribute=\"Attribute value\">"
					"Content"
					"<InnerElement/>"
				"</Element>"
			"</TestDoc>"
			), mTargetStream.str());
	}

	void testDocumentWithFullContentAndIndentation()
	{
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * domInnerElement = mDocument->createElement(X("InnerElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		domElement->setAttribute(X("attribute"),X("Attribute value"));
		domElement->appendChild(domContent);
		domElement->appendChild(domInnerElement);
		mDocument->getDocumentElement()->appendChild(domElement);
		
		XercesDomWriter writer;
		writer.DoIndentedFormat(true);
		writer.write(mTargetStream, mDocument);

		// Xerces does a very weird indentation!!

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
			"<TestDoc xmlns=\"2003-04.clam05.iua.mtg.upf.es\">\n\n"
			"  <Element attribute=\"Attribute value\">"
				"Content\n"
			"    <InnerElement/>\n"
			"  </Element>\n\n"
			"</TestDoc>\n"
			), mTargetStream.str());
	}

};




} // namespace Test
} // namespace Cuidado

#endif // USE_XERCES_TRUE

