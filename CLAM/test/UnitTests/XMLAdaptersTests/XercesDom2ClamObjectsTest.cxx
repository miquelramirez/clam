#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "XMLStorage.hxx"
#include "XmlMockUpObjects.hxx"
#include "Component.hxx"
#include <list>
#include "XercesDomWriter.hxx"
#include "XercesInitializer.hxx"
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
/*
TOTEST:
- Return values on error conditions
- Recursive behaviour for text components
- Recursive behaviour for element components
*/

namespace CLAM
{
namespace Test
{

class XercesDomToClamObjectsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomToClamObjectsTest );

class XercesDomToClamObjectsTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomToClamObjectsTest );

	CPPUNIT_TEST(testLoadingAWordOnBasicAsContent);
	CPPUNIT_TEST(testLoadingTwoWordsOnTwoBasicsAsContent);
	CPPUNIT_TEST(testLoadingOneWordsOnTwoBasicsAsContent_secondLoadFails);
	CPPUNIT_TEST(testEmptyElement_WithRightLabel);
	CPPUNIT_TEST(testEmptyElement_WithWrongLabel);
	CPPUNIT_TEST(testLoadingElementContent);
	CPPUNIT_TEST(testLoadingContentAfterElement);
	CPPUNIT_TEST(testLoadingSubobjects);
	CPPUNIT_TEST(testLoadingSubobjects_whenComponentIsContent);
	CPPUNIT_TEST(testForErrors_UnexpectedElementOnRoot);
	CPPUNIT_TEST(testForErrors_UnexpectedContentOnRoot);
	CPPUNIT_TEST(testForErrors_UnexpectedElementOnInner);
	CPPUNIT_TEST(testForErrors_UnexpectedContentOnInner);
	CPPUNIT_TEST(testForErrors_withXmlParseErrors);
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

	void assertComponentsLoaded(const std::string & expected, CompositeOfXmlables & toLoad, std::istream & xml)
	{
		XMLStorage loader;
		loader.Read(xml);
		loader.RestoreObject(toLoad);
		loader.ThrowErrors();

		CPPUNIT_ASSERT_EQUAL(expected, toLoad.childStructureTrace(0));
	}
	void assertErrorGiven(const std::string & expected, CompositeOfXmlables & toLoad, std::istream & xml, const std::string & parsed="")
	{
		XMLStorage loader;
		try
		{
			loader.Read(xml);
			loader.RestoreObject(toLoad);
			loader.ThrowErrors();
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::XmlStorageErr & err)
		{
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(parsed, toLoad.childStructureTrace(0));
	}
	
	
	std::stringstream mTargetStream;

	void testLoadingAWordOnBasicAsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"Content"
			"</Doc>"
			);
		
		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic;
		basic.setContent("PreviousValue");
		toLoad.add(basic);

		std::string expected= "B'Content'\n";

		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingTwoWordsOnTwoBasicsAsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"Content1 Content2"
			"</Doc>"
			);
		
		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		toLoad.add(basic1);
		toLoad.add(basic2);

		std::string expected= 
			"B'Content1'\n"
			"B'Content2'\n"; 
		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingOneWordsOnTwoBasicsAsContent_secondLoadFails()
	{
		std::istringstream xml(
			"<Doc>"
			"Content1"
			"</Doc>"
			);
		
		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic1;
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		toLoad.add(basic1);
		toLoad.add(basic2);

		std::string expected= 
			"B'Content1'\n"
			"B''[unloaded]\n"; 
		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testEmptyElement_WithRightLabel()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element/>"
			"</Doc>"
			);

		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic("Element",true);
		toLoad.add(basic);

		std::string expected= "B''\n";

		assertComponentsLoaded(expected, toLoad, xml);
	}
	void testEmptyElement_WithWrongLabel()
	{
		std::istringstream xml(
			"<Doc>"
			"<Other/>"
			"</Doc>"
			);

		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic("Element",true);
		basic.setContent("PreviousValue");
		XmlMockUpBasic basic2("Other",true);
		basic2.setContent("PreviousOtherValue");
		toLoad.add(basic);
		toLoad.add(basic2);

		std::string expected=
			"B'PreviousValue'[unloaded]\n"
			"B''\n";

		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingElementContent()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>ElementContent</Element>"
			"</Doc>"
			);
		
		CompositeOfXmlables toLoad;
		XmlMockUpComponent component("Element",true);
		component.setContent("PeviousValue1");
		toLoad.add(component);

		std::string expected= "C'ElementContent'\n{\n}\n"; 
		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingContentAfterElement()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element/>"
			"AfterElementContent"
			"</Doc>"
			);
		
		CompositeOfXmlables toLoad;
		XmlMockUpBasic basic1("Element",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		toLoad.add(basic1);
		toLoad.add(basic2);

		std::string expected=
			"B''\n"
			"B'AfterElementContent'\n"; 
		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingSubobjects()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"<SubElement/>"
			"AfterElementContent"
			"</Element>"
			"</Doc>"
			);

		CompositeOfXmlables toLoad;
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic1("SubElement",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		component.add(basic1);
		component.add(basic2);
		toLoad.add(component);

		std::string expected= 
			"C''\n"
			"{\n"
			".B''\n" 
			".B'AfterElementContent'\n"
			"}\n";

		assertComponentsLoaded(expected, toLoad, xml);
	}

	void testLoadingSubobjects_whenComponentIsContent()
	{
		std::istringstream xml(
			"<Doc>"
			"<SubElement/>"
			"AfterElementContent"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;
		XmlMockUpComponent component;
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic1("SubElement",true);
		basic1.setContent("PreviousValue1");
		XmlMockUpBasic basic2;
		basic2.setContent("PreviousValue2");
		component.add(basic1);
		component.add(basic2);
		toLoad.add(component);

		std::string expected= 
			"C''[unloaded]\n" // because the component content is empty
			"{\n"
			".B''\n" 
			".B'AfterElementContent'\n"
			"}\n";

		assertComponentsLoaded(expected, toLoad, xml);
	}
	void testForErrors_UnexpectedElementOnRoot()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"</Element>"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;

		std::string expected= 
			"Unexpected Element: 'Element' at position /Doc\n";
		std::string parsed="";

		assertErrorGiven(expected, toLoad, xml, parsed);
	}

	void testForErrors_UnexpectedContentOnRoot()
	{
		std::istringstream xml(
			"<Doc>"
			"Unexpected content"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;

		std::string expected= 
			"Unexpected content: 'Unexpected content' at position /Doc\n";
		std::string parsed= "";

		assertErrorGiven(expected, toLoad, xml, parsed);
	}
	void testForErrors_UnexpectedElementOnInner()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"<UnExpectedSubElement/>"
			"</Element>"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		toLoad.add(component);

		std::string expected= 
			"Unexpected Element: 'UnExpectedSubElement' at position /Doc/Element\n";
		std::string parsed= 
			"C''\n"
			"{\n"
			"}\n";

		assertErrorGiven(expected, toLoad, xml, parsed);
	}

	void testForErrors_UnexpectedContentOnInner()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"ParsedWord Unexpected Content"
			"</Element>"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		toLoad.add(component);

		std::string expected= 
			"Unexpected content: 'Unexpected Content' at position /Doc/Element\n";
		std::string parsed= 
			"C'ParsedWord'\n"
			"{\n"
			"}\n";

		assertErrorGiven(expected, toLoad, xml, parsed);
	}
	void testForErrors_withXmlParseErrors()
	{
		std::istringstream xml(
			"<Doc>"
			"</Element>"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;

		std::string expected= 
			"\nXML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 8:\n"
			"Expected end of tag 'Doc'\n";
		std::string parsed= "";

		assertErrorGiven(expected, toLoad, xml, parsed);
	}

};




} // namespace Test
} // namespace Cuidado

