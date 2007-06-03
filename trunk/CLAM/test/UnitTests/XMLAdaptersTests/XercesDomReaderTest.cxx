#ifdef USE_XERCES

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XercesDomWriter.hxx" // CLAM
#include "XercesDomReader.hxx" // CLAM
#include "XercesInitializer.hxx" // CLAM
#include "XercesEncodings.hxx" // CLAM
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

class XercesDomReaderTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomReaderTest );

class XercesDomReaderTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomReaderTest );
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
		XercesInitializer::require();
		mTargetStream.str("");
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:

	std::stringstream mTargetStream;
	std::string xmlHeader() 
	{
		return "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
	}

	void testBadSource_throwsException()
	{
		std::ifstream toRead("NonExistingFile.xml");
		XercesDomReader reader;
		try {
			xercesc::DOMNode * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
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
		XercesDomReader reader;

		try {
			xercesc::DOMNode * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string(
					"\n"
					"XML Parser Errors:\n"
					"Fatal Error at file CLAMParser, line 1, col 1:\n"
					"Invalid document structure\n"
					),
			std::string(e.what()));
		}
	}
	void testEmptyDocument()
	{
		std::istringstream toRead("<EmptyDocument />");
		XercesDomReader reader;
		xercesc::DOMDocument * doc = reader.read(toRead);
		XercesDomWriter writer;
		writer.write(mTargetStream, doc);

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
		XercesDomReader reader;
		xercesc::DOMDocument * doc = reader.read(toRead);
		XercesDomWriter writer;
		writer.write(mTargetStream, doc);

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
		XercesDomReader reader;
		try {
			xercesc::DOMNode * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
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
		XercesDomReader reader;
		try {
			xercesc::DOMNode * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 81:\n"
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
		XercesDomReader reader;
		try {
			xercesc::DOMNode * document = reader.read(toRead);
			CPPUNIT_FAIL("Read should have thrown an exception");
			document=0; // Use the un used var to stop warnings.
		}
		catch (XmlStorageErr & e)
		{
			CPPUNIT_ASSERT_EQUAL(std::string(
			"\n"
			"XML Parser Errors:\n"
			"Fatal Error at file CLAMParser, line 1, col 71:\n"
			"Expected end of tag 'Element'\n"
			), std::string(e.what()));
		}
	}

};




} // namespace Test
} // namespace Cuidado

#endif //USE_XERCES

