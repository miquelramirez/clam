#ifdef USE_XMLPP

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XMLStorage.hxx" // CLAM
#include "XmlMockUpObjects.hxx" // CLAM
#include "Component.hxx" // CLAM
#include <list>
#include "LibXmlDomReadingContext.hxx" // CLAM
#include "LibXmlDomWriter.hxx" // CLAM
#include "LibXmlDomReader.hxx" // CLAM

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

class LibXmlDomReadingContextTest;

CPPUNIT_TEST_SUITE_REGISTRATION( LibXmlDomReadingContextTest );

class LibXmlDomReadingContextTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( LibXmlDomReadingContextTest );

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
//	CPPUNIT_TEST(testFindElement_withStillNonElement_asserts);
	CPPUNIT_TEST(testFetchElement_withThatElementFirst);
	CPPUNIT_TEST(testFetchElement_withSecondElementFirst);
	CPPUNIT_TEST(testFetchElement_withTextFirst);
	CPPUNIT_TEST(testFetchElement_withNoElement);
	CPPUNIT_TEST(testFetchElement_withADifferentName);
//	CPPUNIT_TEST(testFetchElement_withANonElementNode);
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
		mTargetStream.str("");
		mDocument = new xmlpp::Document;
		mRoot = mDocument->create_root_node(
			"TestDoc" // root element name
//			,"http://iua.upf.es/mtg/clam/0.8.0/2005-06" // root element namespace URI.
//			,"clam" // namespace prefix
		);
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

	void testFetchContent_withASingleWordContent()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content");

		LibXmlDomReadingContext context(contextElement);
		std::string content;
		std::getline(context.reachableContent(), content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}
	void testFetchContent_withTwoJointContents()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content1");
		contextElement->add_child_text("Content2");

		LibXmlDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}
	void testFetchContent_withTwoContentsSeparatedByElement()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content1");
		contextElement->add_child("Separator");
		contextElement->add_child_text("Content2");

		LibXmlDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1"),content);
	}
	void testFetchContent_withTwoContentsSeparatedByComment()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content1");
		contextElement->add_child_comment("Separator");
		contextElement->add_child_text("Content2");

		LibXmlDomReadingContext context(contextElement);
		std::string content;
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content1Content2"),content);
	}
	void testFetchContent_withNoNode()
	{

		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		LibXmlDomReadingContext context(contextElement);
		std::string content="rubbish";

		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}
	void testFetchContent_withElementFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("UnexpectedElement");
		contextElement->add_child_text("Content");

		LibXmlDomReadingContext context(contextElement);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		
		std::getline(stream, content);
		CPPUNIT_ASSERT(stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}
	void testContentLeft_withNoNode_returnsFalse()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");

		LibXmlDomReadingContext context(contextElement);
		std::string content="rubbish";

		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
	}
	void testContentLeft_withNonSpaceReturnsTrue()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content");

		LibXmlDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}
	void testContentLeft_withStartingSpacesAndNonSpace_chopsAndReturnsTrue()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("  \t  \n Content");

		LibXmlDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content);
	}
	void testContentLeft_withOnlySpaces_chopsAndReturnsFalse()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text(" \t \n  ");

		LibXmlDomReadingContext context(contextElement);
		bool charactersLeft = context.contentLeft();
		CPPUNIT_ASSERT(!charactersLeft);
		std::string content="rubbish";
		std::istream & stream = context.reachableContent();
		std::getline(stream, content);
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}
	void testFindElement_withThatElementFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}
	void testFindElement_withWrongNameFails()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Wrong");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}
	void testFindElement_withCommentBefore()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_comment("Separator");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}
	void testFindElement_withNoElement()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");

		LibXmlDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}
	void testFindElement_withTextFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("ContentLeft");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
	}
	void testFindElement_withReadedTextFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("ReadingContent");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("ReadingContent"), content);
	}
	void testFindElement_withHalfReadedTextFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Reading Content");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);

		std::string content;
		context.reachableContent() >> content;

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(!foundElement);
		CPPUNIT_ASSERT_EQUAL(std::string("Reading"), content);
	}
	void testFindElement_withSpacesToReadFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("    \n \t \t \r ");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);

		bool foundElement = context.findElement("Element");
		CPPUNIT_ASSERT(foundElement);
	}
	// TODO: This test can not be implemented because you cannot add PI's with libxml
	/*
	void testFindElement_withStillNonElement_asserts()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_processing_instruction("ProcessingInstruction","Content");

		LibXmlDomReadingContext context(contextElement);

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
	*/
	void testFetchElement_withThatElementFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		xmlpp::Element * domElement = contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		xmlpp::Element * foundElement = context.fetchElement("Element");
		CPPUNIT_ASSERT_EQUAL(domElement, foundElement);
	}
	void testFetchElement_withSecondElementFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		xmlpp::Element * domElement1 = contextElement->add_child("Element1");
		xmlpp::Element * domElement2 = contextElement->add_child("Element2");

		LibXmlDomReadingContext context(contextElement);
		xmlpp::Element * foundElement1 = context.fetchElement("Element1");
		xmlpp::Element * foundElement2 = context.fetchElement("Element2");
		CPPUNIT_ASSERT_EQUAL(domElement2, foundElement2);
		CPPUNIT_ASSERT_EQUAL(domElement1, foundElement1);
	}
	void testFetchElement_withTextFirst()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("ContentLeft");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);
		try 
		{
			xmlpp::Element * foundElement = context.fetchElement("Element");
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
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);

		context.fetchElement("Element");
		try 
		{
			context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Accessing beyond DOM nodes"),
				std::string(e.what()));
		}
	}
	void testFetchElement_withADifferentName()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context(contextElement);

		try 
		{
			context.fetchElement("WrongElement");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("XML element name should be the one expected"),
				std::string(e.what()));
		}
	}
	// TODO: This test can not be implemented because you cannot add PI's with libxml
	/*
	void testFetchElement_withANonElementNode()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_processing_instruction("ProcessingInstruction","Content");

		LibXmlDomReadingContext context(contextElement);

		try 
		{
			context.fetchElement("Element");
			CPPUNIT_FAIL("Should have failed an assertion");
		} 
		catch (ErrAssertionFailed & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Can't change the context to a non element node"),
				std::string(e.what()));
		}
	}
	*/
	void testFetchContent_afterElement()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Content1");
		contextElement->add_child("Element");
		contextElement->add_child_text("Content2");

		LibXmlDomReadingContext context(contextElement);
		std::string content1="rubbish";
		std::getline(context.reachableContent(), content1);
		context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content2"),content2);
	}
	void testFetchContent_whenSecondElement()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element1");
		contextElement->add_child("Element2");

		LibXmlDomReadingContext context(contextElement);
		context.fetchElement("Element1");
		std::istream & stream = context.reachableContent();
		std::string content="rubbish";
		std::getline(stream, content);

		CPPUNIT_ASSERT_EQUAL(true,stream.fail());
		CPPUNIT_ASSERT_EQUAL(std::string(""),content);
	}
	void testFetchContent_afterElementWithSpaces()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");
		contextElement->add_child_text("  Content");

		LibXmlDomReadingContext context(contextElement);
		context.fetchElement("Element");
		std::string content2="rubbish";
		std::getline(context.reachableContent(), content2);

		CPPUNIT_ASSERT_EQUAL(std::string("Content"),content2);
	}
	void testReleaseContext_atRootReturnsNull()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");

		LibXmlDomReadingContext context(contextElement);
		LibXmlDomReadingContext * previous = context.release();
		
		CPPUNIT_ASSERT_EQUAL((LibXmlDomReadingContext*)0, previous);
	}
	void testReleaseContext_whenIsChildContext()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");

		LibXmlDomReadingContext context1(contextElement);
		LibXmlDomReadingContext context2(&context1,"Element");

		LibXmlDomReadingContext * previous = context2.release();
		
		CPPUNIT_ASSERT_EQUAL(&context1, previous);
	}
	void testRecursiveConstructor_initializesTheContext()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		xmlpp::Element * outerElement = contextElement->add_child("Element");
		xmlpp::Element * innerElement = outerElement->add_child("InnerElement");

		LibXmlDomReadingContext contextRoot(contextElement);
		LibXmlDomReadingContext contextInner(&contextRoot,"Element");
		xmlpp::Element * foundElement = contextInner.fetchElement("InnerElement");

		CPPUNIT_ASSERT_EQUAL(innerElement, foundElement);
	}
	void testReleaseContext_whenContentLeft()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child_text("Offending input\none line more");

		LibXmlDomReadingContext context(contextElement);

		context.release();

		std::list<std::string> errors = context.errors();

		std::string result;
		for (std::list<std::string>::iterator it=errors.begin(); it!=errors.end(); it++)
			result+= *it + '\n';

		CPPUNIT_ASSERT_EQUAL(std::string(
			"Unexpected content: 'Offending input\none line more' at position /TestDoc/ContextElement\n"),
			result
			);
	}
	void testReleaseContext_whenElementLeft()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->add_child("Element");
		contextElement->add_child("Offender");
		
		LibXmlDomReadingContext context(contextElement);

		context.fetchElement("Element");
		context.release();

		std::list<std::string> errors = context.errors();

		std::string result;
		for (std::list<std::string>::iterator it=errors.begin(); it!=errors.end(); it++)
			result+= *it + '\n';

		CPPUNIT_ASSERT_EQUAL(std::string(
			"Unexpected Element: 'Offender' at position /TestDoc/ContextElement\n"),
			result
			);
	}
	void testRelease_whenANonElementAndNonContentNodeLeft()
	{
		// TODO
	}

	void testExtractAttribute_whenNone()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		LibXmlDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("Unexistent", stream);

		CPPUNIT_ASSERT_MESSAGE("Should have been false",!result);
	}
	void testExtractAttribute_whenPresent()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->set_attribute("Attribute","AttributeValue");

		LibXmlDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("Attribute", stream);
		
		CPPUNIT_ASSERT_MESSAGE("Should have been true",result);
		
		std::string extractedValue;
		stream >> extractedValue;

		CPPUNIT_ASSERT_EQUAL(std::string("AttributeValue"),extractedValue);
	}

	void testExtractAttribute_withDifferentAttributeName()
	{
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement->set_attribute("Attribute","AttributeValue");

		LibXmlDomReadingContext context(contextElement);
		std::stringstream stream;
		bool result = context.extractAttribute("WrongAttribute", stream);
		
		CPPUNIT_ASSERT_MESSAGE("Should have been false",!result);
	}

	void testGetPath()
	{
		LibXmlDomReader reader;
		xmlpp::Element * contextElement = mRoot->add_child("ContextElement");
		contextElement
			->add_child("Element1")
			->add_child("Element2")
			->add_child("Element3");

		LibXmlDomReadingContext context0(contextElement);
		LibXmlDomReadingContext context1(&context0,"Element1");
		LibXmlDomReadingContext context2(&context1,"Element2");
		LibXmlDomReadingContext context3(&context2,"Element3");

		std::string path = context3.getPath();

		CPPUNIT_ASSERT_EQUAL(std::string("/TestDoc/ContextElement/Element1/Element2/Element3"),path);
	}



};




} // namespace Test
} // namespace Cuidado

#endif//USE_XMLPP

