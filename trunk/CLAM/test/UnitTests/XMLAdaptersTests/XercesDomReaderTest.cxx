#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "XercesDomWriter.hxx"
#include "XercesDomReader.hxx"
#include "XercesEncodings.hxx"
#include <sstream>

namespace CLAM
{
namespace Test
{
/**
 * TODO:
 * - When there are two errors the message is incongruent
 * - Test entity translation ie < > &...
 */

class XercesDomReaderTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomReaderTest );

class XercesDomReaderTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomReaderTest );
	CPPUNIT_TEST(testEmptyDocument);
	CPPUNIT_TEST(testFullDocument);
	CPPUNIT_TEST(testFullDocument_whenDuplicatedAttribute);
	CPPUNIT_TEST(testFullDocument_whenUnclosedTag);
	CPPUNIT_TEST(testFullDocument_whenClosingUnopenedTag);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
		xercesc::XMLPlatformUtils::Initialize();
		mTargetStream.str("");
		xercesc::DOMImplementation * imp = 
			xercesc::DOMImplementation::getImplementation();
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
		xercesc::XMLPlatformUtils::Terminate();
	}

private:

	std::stringstream mTargetStream;
	std::string xmlHeader() 
	{
	return "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
	}

	void testEmptyDocument()
	{
		std::istringstream toRead("<EmptyDocument />");
		XercesDomReader reader(toRead);
		xercesc::DOMDocument * doc = reader.read();
		XercesDomWriter writer(mTargetStream);
		writer.write(doc);

		CPPUNIT_ASSERT_EQUAL(std::string(
		xmlHeader() +
		"<EmptyDocument/>"
		), mTargetStream.str());
	}

	void testFullDocument()
	{
		const char * toParse = 
		"<TestDoc>"
			"<Element attribute=\"Attribute value\">"
				"Content"
				"<InnerElement/>"
			"</Element>"
		"</TestDoc>";
		
		std::istringstream toRead(toParse);
		XercesDomReader reader(toRead);
		xercesc::DOMDocument * doc = reader.read();
		XercesDomWriter writer(mTargetStream);
		writer.write(doc);

		CPPUNIT_ASSERT_EQUAL(xmlHeader()+toParse,
			mTargetStream.str());
	}

	void testFullDocument_whenDuplicatedAttribute()
	{
		const char * toParse = 
		"<TestDoc>"
			"<Element attribute=\"Attribute value\" attribute=\"other value\">"
				"Content"
				"<InnerElement/>"
			"</Element>"
		"</TestDoc>";
		
		std::istringstream toRead(toParse);
		XercesDomReader reader(toRead);
		try {
			xercesc::DOMNode * document = reader.read();
			CPPUNIT_FAIL("Read should have thrown an exception");
		}
		catch (Err & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 71:\n"
			"The attribute 'attribute' is already used in element 'Element'\n"
			), std::string(e.what()));
		}
	}

	void testFullDocument_whenUnclosedTag()
	{
		const char * toParse = 
		"<TestDoc>"
			"<Element attribute=\"Attribute value\">"
				"Content"
				"<Unclosed>"
				"<InnerElement/>"
			"</Element>"
		"</TestDoc>";
		
		std::istringstream toRead(toParse);
		XercesDomReader reader(toRead);
		try {
			xercesc::DOMNode * document = reader.read();
			CPPUNIT_FAIL("Read should have thrown an exception");
		}
		catch (Err & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 88:\n"
			"Expected end of tag 'Unclosed'\n"
			), std::string(e.what()));
		}
	}

	void testFullDocument_whenClosingUnopenedTag()
	{
		const char * toParse = 
		"<TestDoc>"
			"<Element attribute=\"Attribute value\">"
				"Content"
				"<InnerElement/>"
				"</Unopened>"
			"</Element>"
		"</TestDoc>";
		
		std::istringstream toRead(toParse);
		XercesDomReader reader(toRead);
		try {
			xercesc::DOMNode * document = reader.read();
			CPPUNIT_FAIL("Read should have thrown an exception");
		}
		catch (Err & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 79:\n"
			"Expected end of tag 'Element'\n"
			), std::string(e.what()));
		}
	}

};




} // namespace Test
} // namespace Cuidado

