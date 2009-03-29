#ifndef _XML_MOCK_UP_OBJECTS_HXX_
#define _XML_MOCK_UP_OBJECTS_HXX_
#include "BasicXMLable.hxx" // CLAM
#include "Component.hxx" // CLAM
#include <list>

namespace CLAM
{
namespace Test
{

class XmlMockUpBasic : public BasicXMLable
{
	public:
		XmlMockUpBasic(const char * name=0, bool isElement=false)
			: BasicXMLable(name, isElement) 
		{
			_loadOk=true;
		}
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
			_content="";
			str >> _content;
			return str!=NULL;
		
		}
		virtual std::string structureTrace(unsigned level)
		{
			return std::string(level,'.')+"B'"+_content+"'"+(_loadOk?"":"[unloaded]")+"\n";
		}
		void setLoadOk(bool storageLoadReturnValue)
		{
			_loadOk = storageLoadReturnValue;
		}
	protected:
		std::string _content;
		bool _loadOk;
};

class CompositeOfXmlables : public Component
{
	public:
		CompositeOfXmlables() {}
		virtual ~CompositeOfXmlables() {}
		void add(XmlMockUpBasic & part)
		{
			_parts.push_back(&part);
		}
		const char * GetClassName() const { return "CLAMTest::CompositeOfXmlables";}
		void StoreOn(Storage & store) const
		{
			std::list<XmlMockUpBasic*>::const_iterator it = _parts.begin();
			for (; it!= _parts.end();it++)
				store.Store(**it);
		}
		void LoadFrom(Storage & store)
		{
			std::list<XmlMockUpBasic *>::const_iterator it = _parts.begin();
			for (; it!= _parts.end();it++)
			{
				bool result = store.Load(**it);
				(*it)->setLoadOk(result);
			}
		}
		std::string childStructureTrace(unsigned childLevel)
		{
			std::string result;
			std::list<XmlMockUpBasic*>::iterator it = _parts.begin();
			for (;it!=_parts.end();it++)
			{
				result+=(*it)->structureTrace(childLevel);
			}
			return result;
		}
	private:
		std::list<XmlMockUpBasic *> _parts;
};


class XmlMockUpComponent : public XmlMockUpBasic, public CompositeOfXmlables 
{
	public:
		XmlMockUpComponent(const char * name=0, bool isElement=false)
			: XmlMockUpBasic(name, isElement) {}
		const char * GetClassName() const { return "CLAMTest::XmlMockUpComponent";}
		virtual std::string structureTrace(unsigned level)
		{
			std::string indentation(level,'.');
			return 
				indentation+"C'"+_content+"'"+(_loadOk?"":"[unloaded]")+"\n"+
				indentation+"{\n"+
				childStructureTrace(level+1) +
				indentation+"}\n";
		}
};


} // namespace Test
} // namespace Cuidado

#endif//_XML_MOCK_UP_OBJECTS_HXX_

