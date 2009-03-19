#ifdef USE_XMLPP

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "LibXmlDomDocumentHandler.hxx" // CLAM
#include <sstream>
#include <iostream>

// TODO: Get the read errors


namespace CLAM
{
namespace Test
{

class LibXmlDomDocumentHandlerTest;

CPPUNIT_TEST_SUITE_REGISTRATION( LibXmlDomDocumentHandlerTest );

class LibXmlDomDocumentHandlerTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( LibXmlDomDocumentHandlerTest );

	CPPUNIT_TEST(testDefaultConstructor_PointsNoWhere);
	CPPUNIT_TEST(testSetDocument_ChangesSelectionToRoot);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testCreate_whenTwice);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testRead_withErrors);
//	CPPUNIT_TEST(testWriteSelection);
	CPPUNIT_TEST(testWriteDocument);
	CPPUNIT_TEST(testSelect_withEmptyPathKeepsTheSelection);
	CPPUNIT_TEST(testSelect_withRelativePathMovesTheSelection);
	CPPUNIT_TEST(testSelect_withRelativePathIgnoringNonElements);
	CPPUNIT_TEST(testSelect_withRelativePathIgnoringElementsWithDifferentNames);
	CPPUNIT_TEST(testSelect_givesExceptionWheneverTheNameIsNotFound);
	CPPUNIT_TEST(testSelect_multipleLevelSelection);
	CPPUNIT_TEST(testSelect_badPathKeepsTheSelection);
	CPPUNIT_TEST(testSelect_withSomeMoreLevels);
	CPPUNIT_TEST(testSelect_successiveRelative);
	CPPUNIT_TEST(testSelect_withAbsolutePath);
	CPPUNIT_TEST(testSelect_absoluteWithBadRootNameFails);
	CPPUNIT_TEST(testSelect_absoluteFailsKeepsOldSelection);
	CPPUNIT_TEST(testSelect_withASingleBarGoesToRoot);
/*
	CPPUNIT_TEST(testWriteSelection_withAMovedSelection);
*/
	CPPUNIT_TEST(testWriteDocument_withAMovedSelectionBehavesTheSame);
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

	void testDefaultConstructor_PointsNoWhere()
	{
		LibXmlDomDocumentHandler doc;

		CPPUNIT_ASSERT_EQUAL((xmlpp::Element*)0,doc.getSelection());
	}
	void testSetDocument_ChangesSelectionToRoot()
	{
		xmlpp::Document * domDoc = new xmlpp::Document;
		xmlpp::Element * root = domDoc->create_root_node("RootNode");

		LibXmlDomDocumentHandler doc;
		doc.setDocument(domDoc);

		CPPUNIT_ASSERT_EQUAL(root,doc.getSelection());
	}
	void testCreate()
	{
		LibXmlDomDocumentHandler doc;
		doc.create("RootNode");

		std::string result=doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("RootNode"),result);
	}
	void testCreate_whenTwice()
	{
		LibXmlDomDocumentHandler doc;
		doc.create("RootNode");
		doc.create("RootNode2");

		std::string result=doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("RootNode2"),result);
	}
	void testRead()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}
	void testRead_withErrors()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><UnclossedChild></LoadedRoot>");
		try
		{
			doc.read(stream);
			CPPUNIT_FAIL("Should throw an exception");
		}
		catch(CLAM::XmlStorageErr & err)
		{
			std::string expected =
				"\nXML Parser Errors:\n"
				"Document not well-formed.\n";
			CPPUNIT_ASSERT_EQUAL(expected,std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL((xmlpp::Element*)0,doc.getSelection());
	}
/*
	void testWriteSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Child/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeSelection(os);

		CPPUNIT_ASSERT_EQUAL(is.str(),os.str());

	}
*/
	void testWriteDocument()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Child/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeDocument(os);

		CPPUNIT_ASSERT_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+is.str()+"\n",os.str());

	}
	void testSelect_withEmptyPathKeepsTheSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}
	void testSelect_withRelativePathMovesTheSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");
		doc.read(stream);

		doc.selectPath("Child");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}
	void testSelect_withRelativePathIgnoringNonElements()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot>ToBe Ignored <!-- Ignored Comment --><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Child");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}
	void testSelect_withRelativePathIgnoringElementsWithDifferentNames()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Ignored /><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Child");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}
	void testSelect_givesExceptionWheneverTheNameIsNotFound()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);
		try
		{
			doc.selectPath("LostChild");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::XmlStorageErr & err)
		{
			std::string expected = "Wrong path step 'LostChild'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}
	void testSelect_multipleLevelSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/Child");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}
	void testSelect_badPathKeepsTheSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		try
		{
			doc.selectPath("Element/BadChild");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::XmlStorageErr & err)
		{
			std::string expected = "Wrong path step 'BadChild'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}
	void testSelect_withSomeMoreLevels()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child><SubChild /></Child></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/Child/SubChild");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("SubChild"),result);
	}
	void testSelect_successiveRelative()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child><SubChild /></Child></Element></LoadedRoot>");
		doc.read(stream);

		doc.selectPath("Element/Child");
		doc.selectPath("SubChild");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("SubChild"),result);
	}
	void testSelect_withAbsolutePath()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("/LoadedRoot/Element/Child");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_absoluteWithBadRootNameFails()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element");
		try
		{
			doc.selectPath("/BadRoot/Element/Child");
			CPPUNIT_FAIL("Should throw an exception");
		}
		catch (CLAM::XmlStorageErr & err)
		{
			std::string expected = "Wrong root name, expected 'BadRoot' but found 'LoadedRoot'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("Element"),result);
	}

	void testSelect_absoluteFailsKeepsOldSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /><OtherChild /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/OtherChild");
		try
		{
			doc.selectPath("/LoadedRoot/Element/LostChild");
			CPPUNIT_FAIL("Should throw an exception");
		}
		catch (CLAM::XmlStorageErr & err)
		{
			std::string expected = "Wrong path step 'LostChild'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("OtherChild"),result);
	}
	void testSelect_withASingleBarGoesToRoot()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /><OtherChild /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/OtherChild");
		doc.selectPath("/");

		std::string result = doc.getSelection()->get_name();

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}
/*
	void testWriteSelection_withAMovedSelection()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Element><Child/><OtherChild/></Element><OtherElement/></LoadedRoot>");

		doc.read(is);
		doc.selectPath("Element");

		std::ostringstream os;
		doc.writeSelection(os);

		std::string expected = "<Element><Child/><OtherChild/></Element>";

		CPPUNIT_ASSERT_EQUAL(expected,os.str());
	}
*/
	void testWriteDocument_withAMovedSelectionBehavesTheSame()
	{
		LibXmlDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Element><Child/><OtherChild/></Element><OtherElement/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeDocument(os);

		CPPUNIT_ASSERT_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+is.str()+"\n",os.str());
	}

};

} // namespace Test
} // namespace Cuidado

#endif//USE_XMLPP


