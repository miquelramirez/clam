/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

//////////////////////////////////////////////////////////////////////
// XMLStorage.cxx
// Description: A storage in XML format for CLAM objects
//////////////////////////////////////////////////////////////////////

#include "XMLStorage.hxx"


#include "XMLable.hxx"
#include "Component.hxx"
#include "Assert.hxx"
#include "XercesDomDocumentHandler.hxx"
#include "XercesDomWritingContext.hxx"
#include "XercesDomReadingContext.hxx"
#include <fstream>

namespace CLAM
{
	void XmlStorage::Read(std::istream & is)
	{
		_documentHandler->read(is);
	}

	void XmlStorage::Create(const std::string name)
	{
		_documentHandler->create(name.c_str());
		_lastWasContent=false;
	}

	void XmlStorage::WriteSelection(std::ostream & os)
	{
		_documentHandler->writeSelection(os,_useIndentation);
	}

	void XmlStorage::WriteDocument(std::ostream & os)
	{
		_documentHandler->writeDocument(os,_useIndentation);
	}

	void XmlStorage::DumpObject(const Component & component)
	{
		XercesDomWritingContext rootContext(*_documentHandler);
		_writeContext = & rootContext;
		component.StoreOn(*this);
	}

	void XmlStorage::RestoreObject(Component & component)
	{
		XercesDomReadingContext rootContext(*_documentHandler);
		_readContext = & rootContext;
		component.LoadFrom(*this);
	}

	void XmlStorage::Select(const std::string & path)
	{
		CLAM_ASSERT(false,"XmlStorage::Select not implemented yet");
	}

	void XmlStorage::UseIndentation(bool useIndentation) {
		_useIndentation=useIndentation;
	}

// Static sumarized interface
	XmlStorage::XmlStorage()
	{
		_documentHandler=new XercesDomDocumentHandler;
		_readContext = 0;
		_writeContext = 0;
		_lastWasContent = true;
		_useIndentation = false;
	}
	XmlStorage::~XmlStorage()
	{
		delete _documentHandler;
	}

	void XmlStorage::Restore(Component & obj, const std::string & filename)
	{
		std::ifstream is(filename.c_str());
		CLAM_WARNING(is.is_open(),"Restoring from an unopened file");
		Restore(obj,is);
	}

	void XmlStorage::Dump(const Component & obj, const std::string & rootName, const std::string & filename)
	{
		std::ofstream os(filename.c_str());
		CLAM_WARNING(os.is_open(),"Dumping on an unopened file");
		Dump(obj,rootName,os);
	}

	void XmlStorage::AppendToDocument(const Component & obj, const std::string & path, std::iostream & str)
	{
		XmlStorage storage;
		storage.Read(str);
		storage.Select(path);
		storage.DumpObject(obj);
		storage.WriteDocument(str);
	}


// Interface for Components to load/store their subitems
	void XmlStorage::Store(const Storable & storable)
	{
		const XMLable * xmlable = dynamic_cast<const XMLable *>(&storable);
		const char * name = xmlable->XMLName();
		if (!name)
		{
			StoreContentAndChildren(xmlable);
			return;
		}
		if (xmlable->IsXMLAttribute())
		{
			_writeContext->addAttribute(name,xmlable->XMLContent().c_str());
			return;
		}
		if (xmlable->IsXMLElement())
		{
			_lastWasContent=false;
			XercesDomWritingContext newContext(_writeContext, name);
			_writeContext = & newContext;
			StoreContentAndChildren(xmlable);
			_writeContext = newContext.release();
			_lastWasContent=false;
			return;
		}
		CLAM_ASSERT(false,"A weird XMLable inserted");
	}

	bool XmlStorage::Load(Storable & storable)
	{
		XMLable * xmlable = dynamic_cast<XMLable *>(&storable);
		if (!xmlable) return false;

		if (xmlable->IsXMLText())
			return LoadContentAndChildren(xmlable);

		if (xmlable->IsXMLElement())
		{
			if (!_readContext->findElement(xmlable->XMLName()))
				return false;
			XercesDomReadingContext innerContext(_readContext, xmlable->XMLName());
			_readContext = &innerContext;
			LoadContentAndChildren(xmlable);
			_readContext = innerContext.release();
		//	addErrors(innerContext.errors());
			return true;
		}

		// TODO: Test Attributes
		if (xmlable->IsXMLAttribute())
		{
			std::stringstream stream;
			if (!_readContext->extractAttribute(xmlable->XMLName(), stream))
				return false;
			return xmlable->XMLContent(stream);
		}

		CLAM_ASSERT(false, "A weird XMLable inserted");
		return false;
	}

// Private helper functions

	bool XmlStorage::LoadContentAndChildren(XMLable* xmlable)
	{
		bool result = xmlable->XMLContent(_readContext->reachableContent());
		Component * component = dynamic_cast<Component*>(xmlable);
		if (component) component->LoadFrom(*this);
		return result;
	}

	void XmlStorage::StoreContentAndChildren(const XMLable * xmlable)
	{
		AddContentToElement(xmlable->XMLContent());
		StoreChildrenIfComponent(xmlable);
	}

	void XmlStorage::StoreChildrenIfComponent(const XMLable * xmlable)
	{
		const Component * component = dynamic_cast<const Component *>(xmlable);
		if (component) component->StoreOn(*this);
	}

	void XmlStorage::AddContentToElement(const std::string & content)
	{
		if (content=="") return;
		if (_lastWasContent)
			_writeContext->addContent(" ");
		_writeContext->addContent(content.c_str());
		_lastWasContent = true;
	}

}

