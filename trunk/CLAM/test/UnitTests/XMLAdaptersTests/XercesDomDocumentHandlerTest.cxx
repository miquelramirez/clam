#ifdef USE_XERCES_TRUE

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XercesDomDocumentHandler.hxx" // CLAM
#include "XercesInitializer.hxx" // CLAM
#include <sstream>
#include <xercesc/dom/DOMImplementation.hpp>
#include "XercesEncodings.hxx" // CLAM
#include <iostream>

// TODO: Get the read errors


namespace xercesc = XERCES_CPP_NAMESPACE;

namespace CLAM
{
namespace Test
{

class XercesDomDocumentHandlerTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XercesDomDocumentHandlerTest );

class XercesDomDocumentHandlerTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XercesDomDocumentHandlerTest );

	CPPUNIT_TEST(testDefaultConstructor_PointsNoWhere);
	CPPUNIT_TEST(testSetDocument_ChangesSelectionToRoot);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testCreate_whenTwice);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testRead_withErrors);
	CPPUNIT_TEST(testWriteSelection);
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
	CPPUNIT_TEST(testWriteSelection_withAMovedSelection);
	CPPUNIT_TEST(testWriteDocument_withAMovedSelectionBehavesTheSame);

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


	void testDefaultConstructor_PointsNoWhere()
	{
		XercesDomDocumentHandler doc;

		CPPUNIT_ASSERT_EQUAL((xercesc::DOMElement*)0,doc.getSelection());
	}

	void testSetDocument_ChangesSelectionToRoot()
	{
		xercesc::DOMImplementation * imp =
			xercesc::DOMImplementation::getImplementation();
		xercesc::DOMDocument * domDoc = imp->createDocument(0,X("RootNode"),0);

		XercesDomDocumentHandler doc;
		doc.setDocument(domDoc);

		CPPUNIT_ASSERT_EQUAL(domDoc->getDocumentElement(),doc.getSelection());
	}

	void testCreate()
	{
		XercesDomDocumentHandler doc;
		doc.create("RootNode");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("RootNode"),result);
	}

	void testCreate_whenTwice()
	{
		XercesDomDocumentHandler doc;
		doc.create("RootNode");
		doc.create("RootNode2");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("RootNode2"),result);
	}

	void testRead()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}

	void testRead_withErrors()
	{
		XercesDomDocumentHandler doc;
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
				"Fatal Error at file CLAMParser, line 1, col 31:\n"
				"Expected end of tag 'UnclossedChild'\n";
			CPPUNIT_ASSERT_EQUAL(expected,std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL((xercesc::DOMElement*)0,doc.getSelection());

	}

	void testWriteSelection()
	{
		XercesDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Child/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeSelection(os);

		CPPUNIT_ASSERT_EQUAL(is.str(),os.str());

	}

	void testWriteDocument()
	{
		XercesDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Child/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeDocument(os);

		CPPUNIT_ASSERT_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"+is.str(),os.str());

	}

	void testSelect_withEmptyPathKeepsTheSelection()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}

	void testSelect_withRelativePathMovesTheSelection()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Child");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_withRelativePathIgnoringNonElements()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot>ToBe Ignored <!-- Ignored Comment --><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Child");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_withRelativePathIgnoringElementsWithDifferentNames()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Ignored /><Child /></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Child");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_givesExceptionWheneverTheNameIsNotFound()
	{
		XercesDomDocumentHandler doc;
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

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}

	void testSelect_multipleLevelSelection()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/Child");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_badPathKeepsTheSelection()
	{
		XercesDomDocumentHandler doc;
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

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}

	void testSelect_withSomeMoreLevels()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child><SubChild /></Child></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/Child/SubChild");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("SubChild"),result);
	}

	void testSelect_successiveRelative()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child><SubChild /></Child></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/Child");
		doc.selectPath("SubChild");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("SubChild"),result);
	}
	void testSelect_withAbsolutePath()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("/LoadedRoot/Element/Child");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Child"),result);
	}

	void testSelect_absoluteWithBadRootNameFails()
	{
		XercesDomDocumentHandler doc;
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

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("Element"),result);
	}

	void testSelect_absoluteFailsKeepsOldSelection()
	{
		XercesDomDocumentHandler doc;
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

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("OtherChild"),result);
	}

	void testSelect_withASingleBarGoesToRoot()
	{
		XercesDomDocumentHandler doc;
		std::istringstream stream("<LoadedRoot><Element><Child /><OtherChild /></Element></LoadedRoot>");

		doc.read(stream);

		doc.selectPath("Element/OtherChild");
		doc.selectPath("/");

		std::string result(L(doc.getSelection()->getNodeName()));

		CPPUNIT_ASSERT_EQUAL(std::string("LoadedRoot"),result);
	}

	void testWriteSelection_withAMovedSelection()
	{
		XercesDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Element><Child/><OtherChild/></Element><OtherElement/></LoadedRoot>");

		doc.read(is);
		doc.selectPath("Element");

		std::ostringstream os;
		doc.writeSelection(os);

		std::string expected = "<Element><Child/><OtherChild/></Element>";

		CPPUNIT_ASSERT_EQUAL(expected,os.str());
	}
	void testWriteDocument_withAMovedSelectionBehavesTheSame()
	{
		XercesDomDocumentHandler doc;
		std::istringstream is("<LoadedRoot><Element><Child/><OtherChild/></Element><OtherElement/></LoadedRoot>");

		doc.read(is);

		std::ostringstream os;
		doc.writeDocument(os);

		CPPUNIT_ASSERT_EQUAL("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"+is.str(),os.str());
	}


};




} // namespace Test
} // namespace Cuidado

#endif // USE_XERCES_TRUE

