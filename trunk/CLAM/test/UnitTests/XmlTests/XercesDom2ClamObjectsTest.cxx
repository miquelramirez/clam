#ifdef USE_XERCES

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XMLStorage.hxx" // CLAM
#include "XmlMockUpObjects.hxx"
#include <fstream>
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
	CPPUNIT_TEST(testLoading_OnAInnerSelection);
	CPPUNIT_TEST(testAppendToDocumentUsingPrimitives);
	CPPUNIT_TEST(testAppendToDocument);
	CPPUNIT_TEST(testRestoreFromFragmentUsingPrimitives);
	CPPUNIT_TEST(testRestoreFromFragment);
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

		CPPUNIT_ASSERT_EQUAL(expected, toLoad.childStructureTrace(0));
	}
	void assertErrorGiven(const std::string & expected, CompositeOfXmlables & toLoad, std::istream & xml, const std::string & parsed="")
	{
		XMLStorage loader;
		try
		{
			loader.Read(xml);
			loader.RestoreObject(toLoad);
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

	void testLoading_OnAInnerSelection()
	{
		std::istringstream xml(
			"<Doc>"
			"<Element>"
			"<SubElement>Content</SubElement>"
			"</Element>"
			"</Doc>"
			);
		CompositeOfXmlables toLoad;
		XmlMockUpComponent component("SubElement",true);
		component.setContent("PreviousElementContent");
		toLoad.add(component);

		std::string expected= 
			"C'Content'\n"
			"{\n"
			"}\n";

		XMLStorage loader;
		loader.Read(xml);
		loader.Select("/Doc/Element");
		loader.RestoreObject(toLoad);

		CPPUNIT_ASSERT_EQUAL(expected, toLoad.childStructureTrace(0));
	}
	// TODO; On this suite?
	void testAppendToDocumentUsingPrimitives()
	{
		std::istringstream xml(
			"<Doc>"
			"<Recipient>"
			"<OldElement/>"
			"OldElementContent"
			"</Recipient>"
			"</Doc>"
			);

		CompositeOfXmlables toAppend;
		XmlMockUpComponent component("Element",true);
		XmlMockUpBasic basic("SubElement",true);
		basic.setContent("SubElementContent");
		component.add(basic);
		XmlMockUpComponent component2("Element2",true);
		toAppend.add(component);
		toAppend.add(component2);

		XmlStorage storage;
		storage.Read(xml);
		storage.Select("Recipient");
		storage.DumpObject(toAppend);
		std::ostringstream os;
		storage.WriteDocument(os);
		std::string expected =
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
			"<Doc>"
			"<Recipient>"
				"<OldElement/>"
				"OldElementContent"
				"<Element>"
				"<SubElement>"
				"SubElementContent"
				"</SubElement>"
				"</Element>"
				"<Element2/>"
			"</Recipient>"
			"</Doc>";
		CPPUNIT_ASSERT_EQUAL(expected,os.str());
	}
	void testAppendToDocument()
	{
		{
			std::ofstream os("deleteme.xml");
			os <<
				"<Doc>"
				"<Recipient>"
				"<OldElement/>"
				"OldElementContent"
				"</Recipient>"
				"</Doc>"
			;
		}

		CompositeOfXmlables toAppend;
		XmlMockUpComponent component("Element",true);
		XmlMockUpBasic basic("SubElement",true);
		basic.setContent("SubElementContent");
		component.add(basic);
		XmlMockUpComponent component2("Element2",true);
		toAppend.add(component);
		toAppend.add(component2);

		XmlStorage::AppendToDocument(toAppend, "/Doc/Recipient", "deleteme.xml");

		std::string expected =
			"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
			"<Doc>"
			"<Recipient>"
				"<OldElement/>"
				"OldElementContent"
				"<Element>"
				"<SubElement>"
				"SubElementContent"
				"</SubElement>"
				"</Element>"
				"<Element2/>"
			"</Recipient>"
			"</Doc>";

		std::string result;
		std::ifstream is("deleteme.xml");
		for (char buffer[255]; is.get(buffer,255,'\0');)
			result += buffer;
		CPPUNIT_ASSERT_EQUAL(expected,result);
		remove("deleteme.xml");
	}

	void testRestoreFromFragmentUsingPrimitives()
	{
		std::istringstream xml(
			"<Doc>"
			"<Recipient>"
				"<Element>"
				"<SubElement>"
				"SubElementContent"
				"</SubElement>"
				"</Element>"
				"<Element2>"
				"Element2Content"
				"</Element2>"
			"</Recipient>"
			"<OtherElement/>"
			"</Doc>"
			);

		CompositeOfXmlables toExtract;
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic("SubElement",true);
		basic.setContent("PreviousSubElementContent");
		XmlMockUpComponent component2("Element2",true);
		component.setContent("PreviousElement2Content");
		component.add(basic);
		toExtract.add(component);
		toExtract.add(component2);

		XmlStorage storage;
		storage.Read(xml);
		storage.Select("Recipient");
		storage.RestoreObject(toExtract);
		std::string expected = 
			"C''\n"
			"{\n"
			".B'SubElementContent'\n"
			"}\n"
			"C'Element2Content'\n"
			"{\n"
			"}\n";
		CPPUNIT_ASSERT_EQUAL(expected, toExtract.childStructureTrace(0));
	}
	void testRestoreFromFragment()
	{
		std::istringstream xml(
			"<Doc>"
			"<Recipient>"
				"<Element>"
				"<SubElement>"
				"SubElementContent"
				"</SubElement>"
				"</Element>"
				"<Element2>"
				"Element2Content"
				"</Element2>"
			"</Recipient>"
			"<OtherElement/>"
			"</Doc>"
			);

		CompositeOfXmlables toExtract;
		XmlMockUpComponent component("Element",true);
		component.setContent("PreviousElementContent");
		XmlMockUpBasic basic("SubElement",true);
		basic.setContent("PreviousSubElementContent");
		XmlMockUpComponent component2("Element2",true);
		component.setContent("PreviousElement2Content");
		component.add(basic);
		toExtract.add(component);
		toExtract.add(component2);

		XmlStorage::RestoreFromFragment(toExtract,"Recipient",xml);
		std::string expected = 
			"C''\n"
			"{\n"
			".B'SubElementContent'\n"
			"}\n"
			"C'Element2Content'\n"
			"{\n"
			"}\n";
		CPPUNIT_ASSERT_EQUAL(expected, toExtract.childStructureTrace(0));
	}
};




} // namespace Test
} // namespace Cuidado

#endif // USE_XERCES

