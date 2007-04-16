#ifdef USE_XERCES_TRUE

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "XMLStorage.hxx" // CLAM
#include "XmlMockUpObjects.hxx"

namespace CLAM
{
namespace Test
{

class ClamObjectsToXercesDomTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ClamObjectsToXercesDomTest );

class ClamObjectsToXercesDomTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ClamObjectsToXercesDomTest );
	CPPUNIT_TEST(testEmptyDocument);
	CPPUNIT_TEST(testBasicAsPlainContent);
	CPPUNIT_TEST(testBasicAsElement_withoutContent);
	CPPUNIT_TEST(testBasicAsElement_withContent);
	CPPUNIT_TEST(testBasicAsPlainContent_withoutContent);
	CPPUNIT_TEST(testBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsPlainContent);
	CPPUNIT_TEST(testComponentAsElement_withoutContent);
	CPPUNIT_TEST(testComponentAsElement_withContent);
	CPPUNIT_TEST(testComponentAsPlainContent_withoutContent);
	CPPUNIT_TEST(testComponentAsAttribute);
	CPPUNIT_TEST(testSibblingsContentsAndAttributes_getOrderedAsInserted);
	CPPUNIT_TEST(testConsecutiveContents_getSpaceSeparation);
	CPPUNIT_TEST(testNonConsecutiveContents_dontGetSpaceSeparation);
	CPPUNIT_TEST(testSibblingsAttributes_getReordered);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsPlainContent);
	CPPUNIT_TEST(testElementContents_getPrintedBeforeSiblingContent);
	CPPUNIT_TEST(testNodesInsertionAfterComponentElement);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsElement_containingBasicAsElement);
	CPPUNIT_TEST(testComponentAsElement_containingComponentAsElement);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsPlainContent);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsAttribute);
	CPPUNIT_TEST(testComponentAsContent_containingBasicAsElement);
	CPPUNIT_TEST(testComponentAsAttribute_containingAnything_childrenHaveNoEffect);
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

	void assertDumpAs(const Component & toStore, const std::string & expected)
	{
		XMLStorage dumper;
		dumper.Create("Doc");
		dumper.DumpObject(toStore);
		dumper.WriteSelection(mTargetStream);
		CPPUNIT_ASSERT_EQUAL(expected, mTargetStream.str());
	}



	void testEmptyDocument()
	{
		CompositeOfXmlables toStore;

		assertDumpAs(toStore,"<Doc/>");
	}

	void testBasicAsPlainContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic basic;
		basic.setContent("Content");
		toStore.add(basic);

		assertDumpAs(toStore,"<Doc>Content</Doc>");
	}

	void testBasicAsPlainContent_withoutContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic basic;
		toStore.add(basic);

		assertDumpAs(toStore,"<Doc/>");
	}

	void testBasicAsElement_withoutContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic basic("Element",true);
		toStore.add(basic);

		assertDumpAs(toStore,"<Doc><Element/></Doc>");
	}

	void testBasicAsElement_withContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic basic("Element",true);
		basic.setContent("Content");
		toStore.add(basic);

		assertDumpAs(toStore,"<Doc><Element>Content</Element></Doc>");
	}
	
	void testBasicAsAttribute()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic basic("at",false);
		basic.setContent("Content");
		toStore.add(basic);

		assertDumpAs(toStore,"<Doc at=\"Content\"/>");
	}

	void testComponentAsPlainContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent component;
		component.setContent("Content");
		toStore.add(component);

		assertDumpAs(toStore,"<Doc>Content</Doc>");
	}

	void testComponentAsPlainContent_withoutContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent component;
		toStore.add(component);

		assertDumpAs(toStore,"<Doc/>");
	}

	void testComponentAsElement_withoutContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent component("Element",true);
		toStore.add(component);

		assertDumpAs(toStore,"<Doc><Element/></Doc>");
	}

	void testComponentAsElement_withContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent component("Element",true);
		component.setContent("Content");
		toStore.add(component);

		assertDumpAs(toStore,"<Doc><Element>Content</Element></Doc>");
	}
	
	void testComponentAsAttribute()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent component("at",false);
		component.setContent("Content");
		toStore.add(component);

		assertDumpAs(toStore,"<Doc at=\"Content\"/>");
	}



	void testSibblingsContentsAndAttributes_getOrderedAsInserted()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic oneElement("OneElement",true);
		XmlMockUpBasic otherElement("OtherElement",true);
		XmlMockUpBasic content;
		content.setContent("Content");
		toStore.add(oneElement);
		toStore.add(content);
		toStore.add(otherElement);

		assertDumpAs(toStore,"<Doc><OneElement/>Content<OtherElement/></Doc>");
	}
	

	void testConsecutiveContents_getSpaceSeparation()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		content2.setContent("Content2");
		toStore.add(content1);
		toStore.add(content2);

		assertDumpAs(toStore,"<Doc>Content1 Content2</Doc>");
	}

	void testNonConsecutiveContents_dontGetSpaceSeparation()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		XmlMockUpBasic element("Element",true);
		content2.setContent("Content2");
		toStore.add(content1);
		toStore.add(element);
		toStore.add(content2);

		assertDumpAs(toStore,"<Doc>Content1<Element/>Content2</Doc>");
	}

	void testSibblingsAttributes_getReordered()
	{
		CompositeOfXmlables toStore;
		// Not a requirement, just to check the behabiour
		XmlMockUpBasic attribute1("zFirst");
		attribute1.setContent("Content1");
		XmlMockUpBasic attribute2("aSecond");
		attribute2.setContent("Content2");
		toStore.add(attribute1);
		toStore.add(attribute2);

		assertDumpAs(toStore,"<Doc aSecond=\"Content2\" zFirst=\"Content1\"/>");
	}

	void testComponentAsElement_containingBasicAsPlainContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.add(content);
		toStore.add(element);


		assertDumpAs(toStore,"<Doc><Element>Content</Element></Doc>");
	}

	void testElementContents_getPrintedBeforeSiblingContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.setContent("ElementContent");
		element.add(content);
		toStore.add(element);


		assertDumpAs(toStore,"<Doc><Element>ElementContent Content</Element></Doc>");
	}

	void testNodesInsertionAfterComponentElement()
	{
		CompositeOfXmlables toStore;
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic afterContent;
		afterContent.setContent("AfterContent");
		element.add(content);
		toStore.add(element);
		toStore.add(afterContent);


		assertDumpAs(toStore,"<Doc><Element>Content</Element>AfterContent</Doc>");
	}

	void testComponentAsElement_containingBasicAsAttribute()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic attribute("at");
		attribute.setContent("atContent");
		element.add(attribute);
		toStore.add(element);


		assertDumpAs(toStore,"<Doc><Element at=\"atContent\"/></Doc>");
	}

	void testComponentAsElement_containingBasicAsElement()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent componentElement("Component",true);
		componentElement.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentElement.add(basicElement);
		toStore.add(componentElement);

		assertDumpAs(toStore,
			"<Doc><Component>ComponentContent"
			"<Basic>BasicContent</Basic>""</Component></Doc>");
	}

	void testComponentAsElement_containingComponentAsElement()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent outsideElement("Outside",true);
		outsideElement.setContent("ComponentContent");
		XmlMockUpComponent insideElement("Inside",true);
		insideElement.setContent("InsideContent");
		outsideElement.add(insideElement);
		toStore.add(outsideElement);

		assertDumpAs(toStore,
			"<Doc><Outside>ComponentContent"
			"<Inside>InsideContent</Inside></Outside></Doc>");
	}

	void testComponentAsContent_containingBasicAsPlainContent()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicContent;
		basicContent.setContent("BasicContent");
		componentContent.add(basicContent);
		toStore.add(componentContent);

		assertDumpAs(toStore,"<Doc>ComponentContent BasicContent</Doc>");
	}

	void testComponentAsContent_containingBasicAsAttribute()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicAttribute("at");
		basicAttribute.setContent("BasicContent");
		componentContent.add(basicAttribute);
		toStore.add(componentContent);

		assertDumpAs(toStore,"<Doc at=\"BasicContent\">ComponentContent</Doc>");
	}

	void testComponentAsContent_containingBasicAsElement()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentContent.add(basicElement);
		toStore.add(componentContent);

		assertDumpAs(toStore,"<Doc>ComponentContent<Basic>BasicContent</Basic></Doc>");
	}

	void testComponentAsAttribute_containingAnything_childrenHaveNoEffect()
	{
		CompositeOfXmlables toStore;
		XmlMockUpComponent componentAttribute("componentAttribute");
		componentAttribute.setContent("ComponentContent");
		XmlMockUpBasic basicElement("BasicElement",true);
		basicElement.setContent("BasicElementContent");
		XmlMockUpBasic basicAttribute("BasicAttribute",true);
		basicElement.setContent("BasicAttributeContent");
		XmlMockUpBasic basicContent;
		basicContent.setContent("BasicContentContent");
		componentAttribute.add(basicElement);
		componentAttribute.add(basicAttribute);
		componentAttribute.add(basicContent);
		toStore.add(componentAttribute);

		assertDumpAs(toStore,"<Doc componentAttribute=\"ComponentContent\"/>");
	}

};




} // namespace Test
} // namespace Cuidado

#endif //USE_XERCES_TRUE

