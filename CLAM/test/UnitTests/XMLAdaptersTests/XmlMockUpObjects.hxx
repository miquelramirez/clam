#ifndef _XML_MOCK_UP_OBJECTS_HXX_
#define _XML_MOCK_UP_OBJECTS_HXX_
#include "BasicXMLable.hxx"
#include "Component.hxx"
#include <list>

namespace CLAM
{
namespace Test
{

class XmlMockUpBasic : public BasicXMLable
{
	public:
		XmlMockUpBasic(const char * name=0, bool isElement=false)
			: BasicXMLable(name, isElement) {}
		void setContent(const std::string & content)
		{
			_content = content;	
		}
		std::string XMLContent() const
		{
			return _content;
		}

	       //* Extracts the content from the stream.
		bool XMLContent(std::istream & str)
		{
			str >> _content;
			return str!=NULL;
		}
		virtual std::string structureTrace(unsigned level)
		{
			return std::string(level,'.')+"B'"+_content+"'\n";
		}
	protected:
		std::string _content;
};

class XmlMockUpComponent : public XmlMockUpBasic, public Component 
{
	public:
		XmlMockUpComponent(const char * name=0, bool isElement=false)
			: XmlMockUpBasic(name, isElement) {}
		void add(XMLable & part)
		{
			_parts.push_back(&part);
		}
		const char * GetClassName() const { return "CLAMTest::XmlMockUpComponent";}
		void StoreOn(Storage & store) const
		{
			std::list<XMLable *>::const_iterator it = _parts.begin();
			for (; it!= _parts.end();it++)
				store.Store(**it);
		}
		void LoadFrom(Storage & store)
		{
		}

		virtual std::string structureTrace(unsigned level)
		{
			std::string indentation(level,'.');
			return 
				indentation+"C'"+_content+"'\n"+
				indentation+"{\n"+
				childStructureTrace(level+1) +
				indentation+"}\n";
		}
		std::string childStructureTrace(unsigned childLevel)
		{
			std::string result;
			std::list<XMLable *>::iterator it = _parts.begin();
			for (;it!=_parts.end();it++)
			{
				result+=dynamic_cast<XmlMockUpBasic*>(*it)->structureTrace(childLevel);
			}
			return result;
		}
	private:
		std::list<XMLable *> _parts;
};

} // namespace Test
} // namespace Cuidado

#endif//_XML_MOCK_UP_OBJECTS_HXX_

