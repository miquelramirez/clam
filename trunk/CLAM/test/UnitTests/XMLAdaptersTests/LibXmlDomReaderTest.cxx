#ifdef USE_XMLPP

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "LibXmlDomReader.hxx" // CLAM
#include "LibXmlDomWriter.hxx" // CLAM
#include <sstream>
#include <fstream>

namespace CLAM
{
namespace Test
{
/**
 * TODO:
 * - When there are two errors the message is incongruent
 * - Test entity translation ie < > &...
 */

class LibXmlDomReaderTest;

CPPUNIT_TEST_SUITE_REGISTRATION( LibXmlDomReaderTest );

class LibXmlDomReaderTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( LibXmlDomReaderTest );
	CPPUNIT_TEST(testBadSource_throwsException);
	CPPUNIT_TEST(testNonXml_throwsException);
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
		mTargetStream.str("");
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:

	std::stringstream mTargetStream;
	void assertBodyEquals(const std::string & expectedBody)
	{
		std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		CPPUNIT_ASSERT_EQUAL(header + expectedBody + "\n", mTargetStream.str());
	}

	void testBadSource_throwsException()
	{
		std::ifstream toRead("NonExistingFile.xml");
		LibXmlDomReader reader;
		try {
			xmlpp::Document * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0;
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Unable to open the document source"),
			std::string(e.what()));
		}
	}
	void testNonXml_throwsException()
	{
		std::istringstream toRead("ument />");
		LibXmlDomReader reader;

		try {
			xmlpp::Document * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string(
					"\n"
					"XML Parser Errors:\n"
					"Document not well-formed.\n"
					),
			std::string(e.what()));
		}
	}
	void testEmptyDocument()
	{
		std::istringstream toRead("<EmptyDocument />");
		LibXmlDomReader reader;
		xmlpp::Document * doc = reader.read(toRead);
		LibXmlDomWriter writer;
		writer.write(mTargetStream, doc);

		assertBodyEquals("<EmptyDocument/>");
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
		LibXmlDomReader reader;
		xmlpp::Document * doc = reader.read(toRead);
		LibXmlDomWriter writer;
		writer.write(mTargetStream, doc);

		assertBodyEquals(toParse);
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
		LibXmlDomReader reader;
		try {
			xmlpp::Document * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Document not well-formed.\n"
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
		LibXmlDomReader reader;
		try {
			xmlpp::Document * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Document not well-formed.\n"
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
		LibXmlDomReader reader;
		try {
			xmlpp::Document * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Document not well-formed.\n"
			), std::string(e.what()));
		}
	}
};




} // namespace Test
} // namespace Cuidado

#endif//USE_XMLPP
