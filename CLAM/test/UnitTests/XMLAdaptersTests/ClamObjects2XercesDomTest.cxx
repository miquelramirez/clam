#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

//#include "XercesDomWriter.hxx"
//#include "XercesEncodings.hxx"
//#include <xercesc/dom/DOMElement.hpp>
//#include <xercesc/dom/DOMText.hpp>
#include "ClamObjects2XercesDom.hxx"
#include "XmlMockUpObjects.hxx"
#include "BasicXMLable.hxx"
#include "Component.hxx"
#include <list>
#include "XercesDomWriter.hxx"
#include <xercesc/dom/DOMElement.hpp>

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

	void testEmptyDocument()
	{
		ClamObject2XercesDom dumper("Doc");
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testBasicAsPlainContent()
	{
		XmlMockUpBasic basic;
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testBasicAsPlainContent_withoutContent()
	{
		XmlMockUpBasic basic;
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testBasicAsElement_withoutContent()
	{
		XmlMockUpBasic basic("Element",true);
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testBasicAsElement_withContent()
	{
		XmlMockUpBasic basic("Element",true);
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testBasicAsAttribute()
	{
		XmlMockUpBasic basic("at",false);
		basic.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(basic);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}

	void testComponentAsPlainContent()
	{
		XmlMockUpComponent component;
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content</Doc>"), mTargetStream.str());
	}

	void testComponentAsPlainContent_withoutContent()
	{
		XmlMockUpComponent component;
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc/>"), mTargetStream.str());
	}

	void testComponentAsElement_withoutContent()
	{
		XmlMockUpComponent component("Element",true);
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element/></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_withContent()
	{
		XmlMockUpComponent component("Element",true);
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}
	
	void testComponentAsAttribute()
	{
		XmlMockUpComponent component("at",false);
		component.setContent("Content");
		ClamObject2XercesDom dumper("Doc");
		dumper.Store(component);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"Content\"/>"), mTargetStream.str());
	}



	void testSibblingsContentsAndAttributes_getOrderedAsInserted()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic oneElement("OneElement",true);
		XmlMockUpBasic otherElement("OtherElement",true);
		XmlMockUpBasic content;
		content.setContent("Content");
		dumper.Store(oneElement);
		dumper.Store(content);
		dumper.Store(otherElement);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><OneElement/>Content<OtherElement/></Doc>"), mTargetStream.str());
	}
	

	void testConsecutiveContents_getSpaceSeparation()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		content2.setContent("Content2");
		dumper.Store(content1);
		dumper.Store(content2);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content1 Content2</Doc>"), mTargetStream.str());
	}

	void testNonConsecutiveContents_dontGetSpaceSeparation()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic content1;
		content1.setContent("Content1");
		XmlMockUpBasic content2;
		XmlMockUpBasic element("Element",true);
		content2.setContent("Content2");
		dumper.Store(content1);
		dumper.Store(element);
		dumper.Store(content2);
		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>Content1<Element/>Content2</Doc>"), mTargetStream.str());
	}

	void testSibblingsAttributes_getReordered()
	{
		// Not a requirement, just to check the behabiour
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic attribute1("zFirst");
		attribute1.setContent("Content1");
		XmlMockUpBasic attribute2("aSecond");
		attribute2.setContent("Content2");
		dumper.Store(attribute1);
		dumper.Store(attribute2);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc aSecond=\"Content2\" zFirst=\"Content1\"/>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsPlainContent()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.add(content);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element></Doc>"), mTargetStream.str());
	}

	void testElementContents_getPrintedBeforeSiblingContent()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		element.setContent("ElementContent");
		element.add(content);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>ElementContent Content</Element></Doc>"), mTargetStream.str());
	}

	void testNodesInsertionAfterComponentElement()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpBasic content;
		content.setContent("Content");
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic afterContent;
		afterContent.setContent("AfterContent");
		element.add(content);
		dumper.Store(element);
		dumper.Store(afterContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element>Content</Element>AfterContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsAttribute()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent element("Element",true);
		XmlMockUpBasic attribute("at");
		attribute.setContent("atContent");
		element.add(attribute);
		dumper.Store(element);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Element at=\"atContent\"/></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingBasicAsElement()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent componentElement("Component",true);
		componentElement.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentElement.add(basicElement);
		dumper.Store(componentElement);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Component>ComponentContent<Basic>BasicContent</Basic></Component></Doc>"), mTargetStream.str());
	}

	void testComponentAsElement_containingComponentAsElement()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent outsideElement("Outside",true);
		outsideElement.setContent("ComponentContent");
		XmlMockUpComponent insideElement("Inside",true);
		insideElement.setContent("InsideContent");
		outsideElement.add(insideElement);
		dumper.Store(outsideElement);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc><Outside>ComponentContent<Inside>InsideContent</Inside></Outside></Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsPlainContent()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicContent;
		basicContent.setContent("BasicContent");
		componentContent.add(basicContent);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>ComponentContent BasicContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsAttribute()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicAttribute("at");
		basicAttribute.setContent("BasicContent");
		componentContent.add(basicAttribute);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc at=\"BasicContent\">ComponentContent</Doc>"), mTargetStream.str());
	}

	void testComponentAsContent_containingBasicAsElement()
	{
		ClamObject2XercesDom dumper("Doc");
		XmlMockUpComponent componentContent;
		componentContent.setContent("ComponentContent");
		XmlMockUpBasic basicElement("Basic",true);
		basicElement.setContent("BasicContent");
		componentContent.add(basicElement);
		dumper.Store(componentContent);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc>ComponentContent<Basic>BasicContent</Basic></Doc>"), mTargetStream.str());
	}

	void testComponentAsAttribute_containingAnything_childrenHaveNoEffect()
	{
		ClamObject2XercesDom dumper("Doc");
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
		dumper.Store(componentAttribute);

		xercesc::DOMNode * node = dumper.getDom()->getDocumentElement();
		XercesDomWriter writer(mTargetStream);
		writer.write(node);

		CPPUNIT_ASSERT_EQUAL(std::string("<Doc componentAttribute=\"ComponentContent\"/>"), mTargetStream.str());
	}

};




} // namespace Test
} // namespace Cuidado

