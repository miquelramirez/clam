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

#ifndef _XMLStorage_
#define _XMLStorage_

#include "Storage.hxx"
#include <iosfwd>
#include <string>


namespace CLAM
{

class XMLable;
class Component;
class XercesDomDocumentHandler;
class XercesDomWritingContext;
class XercesDomReadingContext;

class XmlStorage : public Storage
{
	XercesDomDocumentHandler * _documentHandler;
	XercesDomWritingContext * _writeContext;
	XercesDomReadingContext * _readContext;
	bool _lastWasContent;
	bool _useIndentation;
public:
	XmlStorage();
	~XmlStorage();

// Final user interface (Atomic operations)
public:
	void Read(std::istream & is);
	void Create(const std::string name);
	void WriteSelection(std::ostream & os);
	void WriteDocument(std::ostream & os);
	void DumpObject(const Component & component);
	void RestoreObject(Component & component);
	/**
	 * @todo Not implemented yet
	 */
	void Select(const std::string & path);
	/**
	 * Changes whether to output pretty formated XML or not.
	 * By default, indentation and new lines are not inserted resulting
	 * in compact XML but dificult for humans to read. Use this method,
	 * to enable the indentation.
	 * @todo Not implemented yet
	 */
	void UseIndentation(bool useIndentation);
// Final User static interface (Summary operations)
public:

	/**
	 * Dumps a Component as XML onto the named file with name as the root element
	 */
	static void Dump(const Component & obj, const std::string & rootName, std::ostream & os)
	{
		XmlStorage storage;
		storage.Create(rootName);
		storage.DumpObject(obj);
		storage.UseIndentation(true);
		storage.WriteSelection(os);
	}

	/**
	 * Restore a Component from the given istream
	 */
	static void Restore(Component & obj, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.RestoreObject(obj);
	}

	/**
	 * Restore a Component from the xml fragment on the given xpath of the given document
	 * @todo Not implemented
	 */
	static void RestorePartialDocument(Component & obj, const std::string & path, std::istream & is)
	{
		XmlStorage storage;
		storage.Read(is);
		storage.Select(path);
		storage.RestoreObject(obj);
	}

	/**
	 * Append the xml fragment corresponding to the given component
	 * on the given xpath of an existing i/o stream
	 * @todo Not implemented
	 */
	static void AppendToDocument(const Component & obj, const std::string & path, std::iostream & str);

	/**
	 * Dump a Component from the named XML file
	 */
	static void Dump(const Component & obj, const std::string & rootName, const std::string & filename);

	/**
	 * Restore a Component from the named XML file
	 */
	static void Restore(Component & obj, const std::string & filename);

// Interface for Components to load/store their subitems
public:
	/**
	 * Components should use that function in their LoadFrom in order to store
	 * their subitems wrapped with XML*Adapters.
	 */
	void Store(const Storable & storable);
	/**
	 * Components should use that function in their LoadFrom in order to load
	 * their subitems wrapped with XML*Adapters.
	 */
	bool Load(Storable & storable);

// Private helper functions
private:
	bool LoadContentAndChildren(XMLable* xmlable);
	void StoreContentAndChildren(const XMLable * xmlable);
	void StoreChildrenIfComponent(const XMLable * xmlable);
	void AddContentToElement(const std::string & content);

};

/** For maintaining compatibility with deprecated class name  */
typedef XmlStorage XMLStorage;

} // namespace CLAM

#endif//_XMLStorage_
