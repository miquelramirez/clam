#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "XercesDomDocumentHandler.hxx"
#include "XercesInitializer.hxx"
#include <sstream>
#include <xercesc/dom/DOMImplementation.hpp>
#include "XercesEncodings.hxx"
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
		catch(...)
		{
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


};




} // namespace Test
} // namespace Cuidado

