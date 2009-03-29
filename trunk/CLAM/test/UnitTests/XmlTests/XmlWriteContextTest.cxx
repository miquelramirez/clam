#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XmlWriteContext.hxx" // CLAM

namespace CLAM
{
namespace Test
{

class XmlWriteContextTest;

CPPUNIT_TEST_SUITE_REGISTRATION( XmlWriteContextTest );

class XmlWriteContextTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( XmlWriteContextTest );
	CPPUNIT_TEST( testCurrentContext_whenNoContextDefinedFails );
	CPPUNIT_TEST( testConstructor_establishCurrentContext );
	CPPUNIT_TEST( testDestructor_removesCurrentContext );
	CPPUNIT_TEST( testConstructor_whenASecondContextDefinedFails );
	CPPUNIT_TEST( testGetTarget );
	CPPUNIT_TEST( testInsertContent_whenString );
	CPPUNIT_TEST( testInsertContent_whenInteger );
	CPPUNIT_TEST( testOpenElement );
	CPPUNIT_TEST( testCloseElement_whenEmpty );
	CPPUNIT_TEST( testCloseElement_whenHasContent );
	CPPUNIT_TEST( testCloseElement_whenHasChildren );
	CPPUNIT_TEST( testCloseElement_whenHasSiblings );
	CPPUNIT_TEST( testLastWasAContent_whenConstructed );
	CPPUNIT_TEST( testLastWasAContent_whenContentWasInserted );
	CPPUNIT_TEST( testLastWasAContent_whenElementOpened );
	CPPUNIT_TEST( testLastWasAContent_whenElementClosed );
	CPPUNIT_TEST( testLastWasAContent_whenContentInsertedOnElement );
	CPPUNIT_TEST( testLastWasAContent_whenElementWithContentClosed );
	CPPUNIT_TEST( testInsertAttribute_whenOutsideTheTagFails );
	CPPUNIT_TEST( testInsertAttribute_whenEmpty );
	CPPUNIT_TEST( testInsertAttribute_whenFull );
	CPPUNIT_TEST( testCurrentLevel_whenConstructed );
	CPPUNIT_TEST( testCurrentLevel_whenContentAdded );
	CPPUNIT_TEST( testCurrentLevel_whenElementOpened );
	CPPUNIT_TEST( testCurrentLevel_whenTwoElementsOpened );
	CPPUNIT_TEST( testCurrentLevel_whenOneElementClosed );
	CPPUNIT_TEST( testCurrentLevel_whenTwoElementClosed );
	CPPUNIT_TEST( testCurrentLevel_whenFirstOfTwoElementClosed );
	CPPUNIT_TEST( testIndentation_whenSingleContent );
	CPPUNIT_TEST( testIndentation_whenSingleElement );
	CPPUNIT_TEST( testIndentation_whenRootSiblings );
	CPPUNIT_TEST( testIndentation_whenNested );
	CPPUNIT_TEST( testIndentation_whenNestedSiblings );
	CPPUNIT_TEST( testIndentation_whenNestedSingleContent );
	CPPUNIT_TEST( testIndentation_whenNestedPreContent );
	CPPUNIT_TEST( testIndentation_whenNestedPostContent );
	CPPUNIT_TEST( testIndentation_whenNestedNested );

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

	void testCurrentContext_whenNoContextDefinedFails()
	{
		try
		{
			XmlWriteContext::CurrentContext();
			CPPUNIT_FAIL("Assertion should have failed");
		}
		catch (CLAM::ErrAssertionFailed e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("XML generation context not created"),
				std::string(e.what()));
		}
	}

	void testConstructor_establishCurrentContext()
	{
		XmlWriteContext theContext(mTargetStream);
		CPPUNIT_ASSERT_EQUAL(&theContext, &XmlWriteContext::CurrentContext());
	}

	void testDestructor_removesCurrentContext()
	{
		{
			XmlWriteContext theContext(mTargetStream);
			// Will be destroyed here
		}
		try
		{
			XmlWriteContext::CurrentContext();
			CPPUNIT_FAIL("Assertion should have failed");
		}
		catch (CLAM::ErrAssertionFailed e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("XML generation context not created"),
				std::string(e.what()));
		}
	}

	void testConstructor_whenASecondContextDefinedFails()
	{
		XmlWriteContext theContext(mTargetStream);
		try
		{
			XmlWriteContext banglerContext(mTargetStream);
			CPPUNIT_FAIL("Assertion should have failed");
		}
		catch (CLAM::ErrAssertionFailed e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("An XML generation context is already defined"),
				std::string(e.what()));
		}
	}

	void testGetTarget()
	{
		XmlWriteContext theContext(mTargetStream);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("mTargetStream is not the one GetTarget returned", 
			static_cast<std::ostream*>(&mTargetStream), 
			&theContext.GetTarget());
	}

	void testLastWasAContent_whenConstructed()
	{
		XmlWriteContext theContext(mTargetStream);
		CPPUNIT_ASSERT_EQUAL(false,theContext.LastWasAContent());
	}

	void testLastWasAContent_whenContentWasInserted()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent(3);
		CPPUNIT_ASSERT_EQUAL(true,theContext.LastWasAContent());
	}

	void testLastWasAContent_whenElementOpened()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent("A dummy text");
		theContext.OpenElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(false,theContext.LastWasAContent());
	}

	void testLastWasAContent_whenElementClosed()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent("A dummy text");
		theContext.OpenElement("MyElement");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(false,theContext.LastWasAContent());
	}

	void testLastWasAContent_whenContentInsertedOnElement()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.InsertContent("A dummy text");
		CPPUNIT_ASSERT_EQUAL(true,theContext.LastWasAContent());
	}

	void testLastWasAContent_whenElementWithContentClosed()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.InsertContent("A dummy text");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(false,theContext.LastWasAContent());
	}

	void testInsertContent_whenInteger()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent(3);
		CPPUNIT_ASSERT_EQUAL(std::string("3"), mTargetStream.str());
	}

	void testInsertContent_whenString()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent("A dummy text");
		CPPUNIT_ASSERT_EQUAL(std::string("A dummy text"), mTargetStream.str());
	}

	void testOpenElement()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(std::string("<MyElement"), mTargetStream.str());
	}

	void testCloseElement_whenEmpty()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(std::string("<MyElement />"), mTargetStream.str());
	}

	void testCloseElement_whenHasContent()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(
			std::string("<MyElement>A dummy content</MyElement>"), 
			mTargetStream.str());
	}

	void testCloseElement_whenHasChildren()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("Child");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("Child");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(
			std::string("<MyElement><Child>A dummy content</Child></MyElement>"), 
			mTargetStream.str());
	}
	
	void testCloseElement_whenHasSiblings()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("Child1");
		theContext.CloseElement("Child1");
		theContext.OpenElement("Child2");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("Child2");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(
			std::string("<MyElement><Child1 /><Child2>A dummy content</Child2></MyElement>"), 
			mTargetStream.str());
	}

	void testInsertAttribute_whenOutsideTheTagFails()
	{
		XmlWriteContext theContext(mTargetStream);
		try {
			theContext.InsertAttribute("anAttribute","value");
			CPPUNIT_FAIL("The assertion should have failed");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(std::string("Appending attribute outside a tag"), std::string(err.what()));
		}
	}

	void testInsertAttribute_whenEmpty()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.InsertAttribute("anAttribute","value");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(
			std::string("<MyElement anAttribute='value' />"), 
			mTargetStream.str());
	}

	void testInsertAttribute_whenFull()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.InsertAttribute("anAttribute","value");
		theContext.OpenElement("Child");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("Child");
		theContext.CloseElement("MyElement");
		CPPUNIT_ASSERT_EQUAL(
			std::string("<MyElement anAttribute='value'><Child>A dummy content</Child></MyElement>"), 
			mTargetStream.str());
	}

	void testCurrentLevel_whenConstructed()
	{
		XmlWriteContext theContext(mTargetStream);

		CPPUNIT_ASSERT_EQUAL(0u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenContentAdded()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.InsertContent("A dummy content");

		CPPUNIT_ASSERT_EQUAL(0u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenElementOpened()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(1u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenTwoElementsOpened()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");

		CPPUNIT_ASSERT_EQUAL(2u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenOneElementClosed()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(0u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenTwoElementClosed()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(0u, theContext.CurrentLevel());
	}

	void testCurrentLevel_whenFirstOfTwoElementClosed()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");

		CPPUNIT_ASSERT_EQUAL(1u, theContext.CurrentLevel());
	}

	void testIndentation_whenSingleContent()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.InsertContent("A dummy content");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"A dummy content"
		), mTargetStream.str());
	}

	void testIndentation_whenSingleElement()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement"
		), mTargetStream.str());
	}

	void testIndentation_whenRootSiblings()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.CloseElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement />\n"
			"<OtherElement />"
		), mTargetStream.str());
	}

	void testIndentation_whenNested()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>\n"
			"\t<OtherElement />\n"
			"</MyElement>"
		), mTargetStream.str());
	}

	void testIndentation_whenNestedSiblings()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");
		theContext.OpenElement("TheElement");
		theContext.CloseElement("TheElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>\n"
			"\t<OtherElement />\n"
			"\t<TheElement />\n"
			"</MyElement>"
		), mTargetStream.str());
	}

	void testIndentation_whenNestedSingleContent()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>A dummy content</MyElement>"
		), mTargetStream.str());
	}

	void testIndentation_whenNestedPreContent()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.InsertContent("A dummy content");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>A dummy content\n"
			"\t<OtherElement />\n"
			"</MyElement>"
		), mTargetStream.str());
	}

	void testIndentation_whenNestedPostContent()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.CloseElement("OtherElement");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>\n"
			"\t<OtherElement />\n"
			"\tA dummy content\n"
			"</MyElement>"
		), mTargetStream.str());
	}

	void testIndentation_whenNestedNested()
	{
		XmlWriteContext theContext(mTargetStream);
		theContext.UseIndentation(true);
		theContext.OpenElement("MyElement");
		theContext.OpenElement("OtherElement");
		theContext.OpenElement("TheElement");
		theContext.InsertContent("A dummy content");
		theContext.CloseElement("TheElement");
		theContext.CloseElement("OtherElement");
		theContext.CloseElement("MyElement");

		CPPUNIT_ASSERT_EQUAL(std::string(
			"<MyElement>\n"
			"\t<OtherElement>\n"
			"\t\t<TheElement>A dummy content</TheElement>\n"
			"\t</OtherElement>\n"
			"</MyElement>"
		), mTargetStream.str());
	}

};




} // namespace Test
} // namespace Cuidado

