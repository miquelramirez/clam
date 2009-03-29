#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XmlMockUpObjects.hxx"

namespace CLAM
{
namespace Test
{

class XmlMockUpObjectsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XmlMockUpObjectsTest );

class XmlMockUpObjectsTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XmlMockUpObjectsTest );

	CPPUNIT_TEST(testTraceStructure_basicWithoutLevelAndContent);
	CPPUNIT_TEST(testTraceStructure_basicWithContent);
	CPPUNIT_TEST(testTraceStructure_basicWithLevel);
	CPPUNIT_TEST(testTraceStructure_emptyComponent);
	CPPUNIT_TEST(testTraceStructure_emptyComponentWithLevel);
	CPPUNIT_TEST(testTraceStructure_componentWithBasicsInside);
	CPPUNIT_TEST(testTraceStructure_componentWithComponentsInside);
	CPPUNIT_TEST(testXMLContentSetter_whenOneWord);
	CPPUNIT_TEST(testXMLContentSetter_whenTwoWords);
	CPPUNIT_TEST(testXMLContentSetter_whenNoWordsGivesDefaultValue);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:
	std::stringstream mTargetStream;

	void testTraceStructure_basicWithoutLevelAndContent()
	{
		XmlMockUpBasic basic;
		std::string expected="B''\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testTraceStructure_basicWithContent()
	{
		XmlMockUpBasic basic;
		basic.setContent("Content");
		std::string expected="B'Content'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testTraceStructure_basicWithLevel()
	{
		XmlMockUpBasic basic;
		basic.setContent("Content");
		std::string expected="..B'Content'\n";

		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(2));
	}

	void testTraceStructure_emptyComponent()
	{
		XmlMockUpComponent component;
		component.setContent("Content");
		std::string expected=
			"C'Content'\n"
			"{\n"
			"}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

	void testTraceStructure_emptyComponentWithLevel()
	{
		XmlMockUpComponent component;
		component.setContent("Content");
		std::string expected=
			"..C'Content'\n"
			"..{\n"
			"..}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(2));
	}

	void testTraceStructure_componentWithBasicsInside()
	{
		XmlMockUpComponent component;
		component.setContent("ComponentContent");
		XmlMockUpBasic basic1;
		basic1.setContent("Basic1Content");
		component.add(basic1);
		XmlMockUpBasic basic2;
		basic2.setContent("Basic2Content");
		component.add(basic2);
		std::string expected=
			"C'ComponentContent'\n"
			"{\n"
			".B'Basic1Content'\n"
			".B'Basic2Content'\n"
			"}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

	void testTraceStructure_componentWithComponentsInside()
	{
		XmlMockUpComponent component;
		XmlMockUpComponent innerComponent;
		XmlMockUpBasic basic1;
		XmlMockUpBasic basic2;
		component.setContent("ComponentContent");
		innerComponent.setContent("InnerComponentContent");
		basic1.setContent("Basic1Content");
		basic2.setContent("Basic2Content");
		component.add(innerComponent);
		innerComponent.add(basic1);
		innerComponent.add(basic2);
		std::string expected=
			"C'ComponentContent'\n"
			"{\n"
			".C'InnerComponentContent'\n"
			".{\n"
			"..B'Basic1Content'\n"
			"..B'Basic2Content'\n"
			".}\n"
			"}\n";

		CPPUNIT_ASSERT_EQUAL(expected, component.structureTrace(0));
	}

	void testXMLContentSetter_whenOneWord()
	{
		XmlMockUpBasic basic;
		basic.setContent("PreviousContent");
		std::istringstream is("NewContent");
		std::string expected="B'NewContent'\n";

		bool result = basic.XMLContent(is);

		CPPUNIT_ASSERT_MESSAGE("XMLContent should have succeded", result);
		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testXMLContentSetter_whenTwoWords()
	{
		XmlMockUpBasic basic;
		basic.setContent("PreviousContent");
		std::istringstream is("NewContent AdditionalContent");
		std::string expected="B'NewContent'\n";

		bool result = basic.XMLContent(is);

		CPPUNIT_ASSERT_MESSAGE("XMLContent should have succeded", result);
		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

	void testXMLContentSetter_whenNoWordsGivesDefaultValue()
	{
		XmlMockUpBasic basic;
		basic.setContent("PreviousContent");
		std::istringstream is("");
		std::string expected="B''\n";

		bool result = basic.XMLContent(is);

		CPPUNIT_ASSERT_MESSAGE("XMLContent should have failed", !result);
		CPPUNIT_ASSERT_EQUAL(expected, basic.structureTrace(0));
	}

};




} // namespace Test
} // namespace Cuidado

