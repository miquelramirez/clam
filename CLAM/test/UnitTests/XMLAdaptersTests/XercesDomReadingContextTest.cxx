#ifdef USE_XERCES

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XmlMockUpObjects.hxx"
#include "Component.hxx" // CLAM
#include <list>
#include "XercesDomWriter.hxx" // CLAM
#include "XercesDomReader.hxx" // CLAM
#include "XercesInitializer.hxx" // CLAM
#include "XercesDomReadingContext.hxx" // CLAM
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
#include <xercesc/dom/DOMImplementation.hpp>

/*
TOTEST:
- A comment doesn't break content
- Any non-comment node does break content
- Spaces at the begining of content are eaten
- Attributes support
*/

namespace CLAM
{
namespace Test
{

class XercesDomReadingContextTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomReadingContextTest );

class XercesDomReadingContextTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomReadingContextTest );

	CPPUNIT_TEST(testFetchContent_withASingleWordContent);
	CPPUNIT_TEST(testFetchContent_withTwoJointContents);
	CPPUNIT_TEST(testFetchContent_withTwoContentsSeparatedByElement);
	CPPUNIT_TEST(testFetchContent_withTwoContentsSeparatedByComment);
	CPPUNIT_TEST(testFetchContent_withNoNode);
	CPPUNIT_TEST(testFetchContent_withElementFirst);
	CPPUNIT_TEST(testContentLeft_withNoNode_returnsFalse);
	CPPUNIT_TEST(testContentLeft_withNonSpaceReturnsTrue);
	CPPUNIT_TEST(testContentLeft_withStartingSpacesAndNonSpace_chopsAndReturnsTrue);
	CPPUNIT_TEST(testContentLeft_withOnlySpaces_chopsAndReturnsFalse);
	CPPUNIT_TEST(testFindElement_withThatElementFirst);
	CPPUNIT_TEST(testFindElement_withWrongNameFails);
	CPPUNIT_TEST(testFindElement_withCommentBefore);
	CPPUNIT_TEST(testFindElement_withNoElement);
	CPPUNIT_TEST(testFindElement_withTextFirst);
	CPPUNIT_TEST(testFindElement_withReadedTextFirst);
	CPPUNIT_TEST(testFindElement_withHalfReadedTextFirst);
	CPPUNIT_TEST(testFindElement_withSpacesToReadFirst);
	CPPUNIT_TEST(testFindElement_withStillNonElement_asserts);
	CPPUNIT_TEST(testFetchElement_withThatElementFirst);
	CPPUNIT_TEST(testFetchElement_withSecondElementFirst);
	CPPUNIT_TEST(testFetchElement_withTextFirst);
	CPPUNIT_TEST(testFetchElement_withNoElement);
	CPPUNIT_TEST(testFetchElement_withADifferentName);
	CPPUNIT_TEST(testFetchElement_withANonElementNode);
	CPPUNIT_TEST(testFetchContent_afterElement);
	CPPUNIT_TEST(testFetchContent_whenSecondElement);
	CPPUNIT_TEST(testFetchContent_afterElementWithSpaces);
	CPPUNIT_TEST(testReleaseContext_atRootReturnsNull);
	CPPUNIT_TEST(testReleaseContext_whenIsChildContext);
	CPPUNIT_TEST(testRecursiveConstructor_initializesTheContext);
	CPPUNIT_TEST(testReleaseContext_whenContentLeft);
	CPPUNIT_TEST(testReleaseContext_whenElementLeft);
	CPPUNIT_TEST(testExtractAttribute_whenNone);
	CPPUNIT_TEST(testExtractAttribute_whenPresent);
	CPPUNIT_TEST(testExtractAttribute_withDifferentAttributeName);
	CPPUNIT_TEST(testGetPath);

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

	void testFetchContent_withASingleWordContent()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::getline(context.reachableContent(), content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testFetchContent_withTwoJointContents()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}

	void testFetchContent_withTwoContentsSeparatedByElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMElement * separator = mDocument->createElement(X("Separator"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(separator);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1"),content);
	}

	void testFetchContent_withTwoContentsSeparatedByComment()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMComment * separator = mDocument->createComment(X("Separator"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(separator);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}

	void testFetchContent_withNoNode()
	{

		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";

		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testFetchContent_withElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * unexpectedElement = mDocument->createElement(X("UnexpectedElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(unexpectedElement);
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT(stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testContentLeft_withNoNode_returnsFalse()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		std::string content="rubbish";

		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
	}

	void testContentLeft_withNonSpaceReturnsTrue()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testContentLeft_withStartingSpacesAndNonSpace_chopsAndReturnsTrue()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("  \t  \n Content"));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}

	void testContentLeft_withOnlySpaces_chopsAndReturnsFalse()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X(" \t \n  "));
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}

	void testFindElement_withThatElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withWrongNameFails()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Wrong"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withCommentBefore()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMComment * separator = mDocument->createComment(X("Separator"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(separator);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withNoElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ContentLeft"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}

	void testFindElement_withReadedTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ReadingContent"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("ReadingContent"), content);
	}

	void testFindElement_withHalfReadedTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Reading Content"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("Reading"), content);
	}

	void testFindElement_withSpacesToReadFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("    \n \t \t \r "));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}

	void testFindElement_withStillNonElement_asserts()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMProcessingInstruction * domProcessingInstruction = 
			mDocument->createProcessingInstruction(X("ProcessingInstruction"),X("Content"));
		contextElement->appendChild(domProcessingInstruction);

		XercesDomReadingContext context(contextElement);

		try
		{
			context.findElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Can't change the context to a non element node"),
				std::string(e.what()));
		}
	}

	void testFetchElement_withThatElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		CPPUNIT_ASSERT_EQUAL(domElement, foundElement);
	}

	void testFetchElement_withSecondElementFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Element1"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element2"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement1 = context.fetchElement("Element1");
		xercesc::DOMElement * foundElement2 = context.fetchElement("Element2");
		CPPUNIT_ASSERT_EQUAL(domElement2, foundElement2);
		CPPUNIT_ASSERT_EQUAL(domElement1, foundElement1);
	}

	void testFetchElement_withTextFirst()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("ContentLeft"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domContent);
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);
		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
			foundElement = 0; // To stop warnings
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Fetching element with content left"),
				std::string(e.what()));
		}
	}

	void testFetchElement_withNoElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		xercesc::DOMElement * foundElement1 = context.fetchElement("Element");
		try 
		{
			xercesc::DOMElement * foundElement2 = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
			foundElement2 = 0; // To stop warnings
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Accessing beyond DOM nodes"),
				std::string(e.what()));
		}
		foundElement1 = 0; // To stop warnings
	}

	void testFetchElement_withADifferentName()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);

		XercesDomReadingContext context(contextElement);

		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("WrongElement");
			CPPUNIT_FAIL("Should have failed an assertion");
			foundElement = 0; // To stop warnings
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("XML element name should be the one expected"),
				std::string(e.what()));
		}
	}
	
	void testFetchElement_withANonElementNode()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMProcessingInstruction * domProcessingInstruction = 
			mDocument->createProcessingInstruction(X("ProcessingInstruction"),X("Content"));
		contextElement->appendChild(domProcessingInstruction);

		XercesDomReadingContext context(contextElement);

		try 
		{
			xercesc::DOMElement * foundElement = context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
			foundElement = 0; // To stop warnings
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Can't change the context to a non element node"),
				std::string(e.what()));
		}
	}


	void testFetchContent_afterElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent1 = mDocument->createTextNode(X("Content1"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMText * domContent2 = mDocument->createTextNode(X("Content2"));
		contextElement->appendChild(domContent1);
		contextElement->appendChild(domElement);
		contextElement->appendChild(domContent2);

		XercesDomReadingContext context(contextElement);
		std::string content1="rubbish";
		std::getline(context.reachableContent(), content1);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content2"),content2);
		foundElement = 0; // To stop warnings
	}

	void testFetchContent_whenSecondElement()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement1 = mDocument->createElement(X("Element1"));
		xercesc::DOMElement * domElement2 = mDocument->createElement(X("Element2"));
		contextElement->appendChild(domElement1);
		contextElement->appendChild(domElement2);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element1");
		std::istream & stream = context.reachableContent();
		std::string content="rubbish";
		std::getline(stream, content);

		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
		foundElement = 0; // To stop warnings
	}

	void testFetchContent_afterElementWithSpaces()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("  Content"));
		contextElement->appendChild(domElement);
		contextElement->appendChild(domContent);

		XercesDomReadingContext context(contextElement);
		xercesc::DOMElement * foundElement = context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content2);
		foundElement = 0; // To stop warnings
	}

	void testReleaseContext_atRootReturnsNull()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));

		XercesDomReadingContext context(contextElement);
		XercesDomReadingContext * previous = context.release();
		
		CPPUNIT_ASSERT_EQUAL((XercesDomReadingContext*)0, previous);
	}

	void testReleaseContext_whenIsChildContext()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		contextElement->appendChild(domElement);
		XercesDomReadingContext context1(contextElement);
		XercesDomReadingContext context2(&context1,"Element");

		XercesDomReadingContext * previous = context2.release();
		
		CPPUNIT_ASSERT_EQUAL(&context1, previous);
	}


	void testRecursiveConstructor_initializesTheContext()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * outerElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * innerElement = mDocument->createElement(X("InnerElement"));
		contextElement->appendChild(outerElement);
		outerElement->appendChild(innerElement);
		XercesDomReadingContext contextRoot(contextElement);
		XercesDomReadingContext contextInner(&contextRoot,"Element");
		xercesc::DOMElement * foundElement = contextInner.fetchElement("InnerElement");

		CPPUNIT_ASSERT_EQUAL(innerElement, foundElement);
		foundElement = 0; // To stop warnings
	}

	void testReleaseContext_whenContentLeft()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMText * domContent = mDocument->createTextNode(X("Offending input\none line more"));
		contextElement->appendChild(domContent);
		XercesDomReadingContext context(contextElement);

		context.release();

		std::list<std::string> errors = context.errors();

		std::string result;
		for (std::list<std::string>::iterator it=errors.begin(); it!=errors.end(); it++)
			result+= *it + '\n';

		CPPUNIT_ASSERT_EQUAL(std::string(
			"Unexpected content: 'Offending input\none line more' at position /ContextElement\n"),
			result
			);
	}

	void testReleaseContext_whenElementLeft()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * domElement = mDocument->createElement(X("Element"));
		xercesc::DOMElement * unparsedElement = mDocument->createElement(X("Offender"));
		contextElement->appendChild(domElement);
		contextElement->appendChild(unparsedElement);
		
		XercesDomReadingContext context(contextElement);

		context.fetchElement("Element");
		context.release();

		std::list<std::string> errors = context.errors();

		std::string result;
		for (std::list<std::string>::iterator it=errors.begin(); it!=errors.end(); it++)
			result+= *it + '\n';

		CPPUNIT_ASSERT_EQUAL(std::string(
			"Unexpected Element: 'Offender' at position /ContextElement\n"),
			result
			);
	}

	void testRelease_whenANonElementAndNonContentNodeLeft()
	{
		// TODO
	}


	void testExtractAttribute_whenNone()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		XercesDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("Unexistent", stream);

		CPPUNIT_ASSERT_MESSAGE("Should have been false",!result);
	}

	void testExtractAttribute_whenPresent()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMAttr * domAttribute = mDocument->createAttribute(X("Attribute"));
		contextElement->setAttribute(X("Attribute"),X("AttributeValue"));
		XercesDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("Attribute", stream);
		
		CPPUNIT_ASSERT_MESSAGE("Should have been true",result);
		
		std::string extractedValue;
		stream >> extractedValue;

		CPPUNIT_ASSERT_EQUAL(std::string("AttributeValue"),extractedValue);
		domAttribute = 0; // To stop
	}

	void testExtractAttribute_withDifferentAttributeName()
	{
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMAttr * domAttribute = mDocument->createAttribute(X("Attribute"));
		contextElement->setAttribute(X("Attribute"),X("AttributeValue"));
		XercesDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("WrongAttribute", stream);
		
		CPPUNIT_ASSERT_MESSAGE("Should have been false",!result);
		domAttribute = 0; // To stop
	}

	void testGetPath()
	{
		XercesDomReader reader;
		xercesc::DOMElement * contextElement = mDocument->createElement(X("ContextElement"));
		xercesc::DOMElement * element1 = mDocument->createElement(X("Element1"));
		xercesc::DOMElement * element2 = mDocument->createElement(X("Element2"));
		xercesc::DOMElement * element3 = mDocument->createElement(X("Element3"));
		contextElement->appendChild(element1);
		element1->appendChild(element2);
		element2->appendChild(element3);

		XercesDomReadingContext context0(contextElement);
		XercesDomReadingContext context1(&context0,"Element1");
		XercesDomReadingContext context2(&context1,"Element2");
		XercesDomReadingContext context3(&context2,"Element3");

		std::string path = context3.getPath();

		CPPUNIT_ASSERT_EQUAL(std::string("/ContextElement/Element1/Element2/Element3"),path);
	}




};




} // namespace Test
} // namespace Cuidado

#endif // USE_XERCE

