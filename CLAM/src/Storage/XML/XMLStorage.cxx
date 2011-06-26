/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#include "Storable.hxx"
#include "Component.hxx"
#include "Assert.hxx"
#include "DomDocumentHandler.hxx"
#include <fstream>
#include <list>
#include <string>
#include <sstream>

namespace CLAM
{
	static std::string join(const std::list<std::string> & strings)
	{
		std::string result;
		std::list<std::string>::const_iterator it;
		for (it=strings.begin(); it!=strings.end(); it++)
		{
			result += *it + "\n";
		}
		return result;
	}
	void XmlStorage::Read(std::istream & is)
	{
		_documentHandler->read(is);
	}

	void XmlStorage::Create(const std::string & name)
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
		DomDocumentHandler::WritingContext rootContext(*_documentHandler);
		_documentHandler->SetWritingContext( & rootContext);
		component.StoreOn(*this);
	}

	void XmlStorage::RestoreObject(Component & component)
	{
		DomDocumentHandler::ReadingContext rootContext(*_documentHandler);
		_documentHandler->SetReadingContext(&rootContext);
		component.LoadFrom(*this);
		_documentHandler->GetReadingContext()->release();
		_errors += join(_documentHandler->GetReadingContext()->errors());
		if (_errors!="")
			throw XmlStorageErr(_errors);
	}

	void XmlStorage::Select(const std::string & path)
	{
		_documentHandler->selectPath(path.c_str());
	}

	void XmlStorage::UseIndentation(bool useIndentation) {
		_useIndentation=useIndentation;
	}

// Static sumarized interface
	XmlStorage::XmlStorage()
	{
		_documentHandler=new DomDocumentHandler;
		_documentHandler->SetReadingContext(0);
		_documentHandler->SetWritingContext(0);
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
		std::ostringstream os;
		os << "Restoring from an unopened file with filename <" << filename << ">" << std::flush;
		CLAM_WARNING(is.is_open(),os.str().c_str());
		Restore(obj,is);
	}

	void XmlStorage::Dump(const Component & obj, const std::string & rootName, const std::string & filename)
	{
		std::ofstream os(filename.c_str());
		CLAM_WARNING(os.is_open(),"Dumping on an unopened file");
		Dump(obj,rootName,os);
	}

	void XmlStorage::AppendToDocument(const Component & obj, const std::string & path, const std::string & filename)
	{
		XmlStorage storage;
		{
			std::ifstream is (filename.c_str());
			storage.Read(is);
		}
		storage.Select(path);
		storage.DumpObject(obj);
		storage.Select("/");
		{
			std::ofstream os(filename.c_str());
			storage.WriteDocument(os);
		}
	}


// Interface for Components to load/store their subitems
	void XmlStorage::Store(const Storable & storable)
	{
		const char * name = storable.XMLName();
		if (!name)
		{
			StoreContentAndChildren(storable);
			return;
		}
		if (storable.IsXMLAttribute())
		{
			_documentHandler->GetWritingContext()->addAttribute(name,storable.XMLContent().c_str());
			return;
		}
		if (storable.IsXMLElement())
		{
			_lastWasContent=false;
			DomDocumentHandler::WritingContext newContext(_documentHandler->GetWritingContext(), name);
			_documentHandler->SetWritingContext(& newContext);
			StoreContentAndChildren(storable);
			_documentHandler->SetWritingContext(newContext.release());
			_lastWasContent=false;
			return;
		}
		CLAM_ASSERT(false,"A weird Storable inserted");
	}

	bool XmlStorage::Load(Storable & storable)
	{
		if (storable.IsXMLText())
			return LoadContentAndChildren(storable);

		if (storable.IsXMLElement())
		{
			if (!_documentHandler->GetReadingContext()->findElement(storable.XMLName()))
				return false;
			DomDocumentHandler::ReadingContext innerContext(_documentHandler->GetReadingContext(), storable.XMLName());
			_documentHandler->SetReadingContext(&innerContext);
			LoadContentAndChildren(storable);
			_documentHandler->SetReadingContext(innerContext.release());
			_errors += join(innerContext.errors());
			return true;
		}

		// TODO: Test Attributes
		if (storable.IsXMLAttribute())
		{
			std::stringstream stream;
			if (!_documentHandler->GetReadingContext()->extractAttribute(storable.XMLName(), stream))
				return false;
			return storable.XMLContent(stream);
		}

		CLAM_ASSERT(false, "A weird Storable inserted");
		return false;
	}

// Private helper functions

	bool XmlStorage::LoadContentAndChildren(Storable & xmlable)
	{
		bool result = xmlable.XMLContent(_documentHandler->GetReadingContext()->reachableContent());
		Component * component = dynamic_cast<Component*>(&xmlable);
		if (component) component->LoadFrom(*this);
		return result;
	}

	void XmlStorage::StoreContentAndChildren(const Storable & xmlable)
	{
		AddContentToElement(xmlable.XMLContent());
		StoreChildrenIfComponent(xmlable);
	}

	void XmlStorage::StoreChildrenIfComponent(const Storable & xmlable)
	{
		const Component * component = dynamic_cast<const Component *>(&xmlable);
		if (component) component->StoreOn(*this);
	}

	void XmlStorage::AddContentToElement(const std::string & content)
	{
		if (content=="") return;
		if (_lastWasContent)
			_documentHandler->GetWritingContext()->addContent(" ");
		_documentHandler->GetWritingContext()->addContent(content.c_str());
		_lastWasContent = true;
	}

}

