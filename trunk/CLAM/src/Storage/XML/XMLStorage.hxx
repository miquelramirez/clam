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

//#include <util/PlatformUtils.hpp>
#include "Storage.hxx"
#include <iosfwd>
#include <string>

namespace CLAM {

class Component;

class XMLStorageImplementation;


class XMLStorage : public Storage {
// Attributes
private:
	XMLStorageImplementation* mPM;
// Construction/Destruction
public:
	XMLStorage();
	virtual ~XMLStorage();
// Redefined methods for Storable
public:
	/**
	 * Add the object at the current storage tree point
	 */
	virtual void Store(Storable * object);
	/**
	 * Retrieves the object from the current storage tree point.
	 */
	virtual bool Load(Storable * object);
// Configuration
	/**
	 * Changes whether to output pretty formated XML or not.
	 * By default, indentation and new lines are not inserted resulting
	 * in compact XML but dificult for humans to read. Use this method,
	 * to enable the indentation.
	 */
	void UseIndentation(bool useIndentation);
// Operators
public:
	/**
	 * Dumps a Component as XML onto the recipient stream with name as the root element
	 */
	void Dump(Component & component, const std::string &name, std::ostream & recipient);

	/**
	 * Dumps a Component as XML onto the named file with name as the root element
	 */
	void Dump(Component & component,
		const std::string& name,
		const std::string& fileName);
	/**
	 * Restore a Component from the specified XML source stream
	 * @todo (Unimplemented) Get the contents storage from a stream.
	 */
	void Restore(Component & component, std::istream & source);
	/**
	 * Restore a Component from the named XML file
	 */
	void Restore(Component & component, const std::string& fileName);
private:
	/**
	 * Factory method that creates a XML Storage Implementation as
	 * configured
	 * @returns The new allocated XML implementation object
	 */
	XMLStorageImplementation * NewXMLImplementation();

// Deprecated
private:
	/**
	 * @deprecated Use the default constructor
	 */
	XMLStorage(const char * rootElementName);
	/**
	 * (Expected to be changed) Dump the storage contents to a stream.
	 * @deprecated by Dump
	 */
	void dumpOn(std::ostream & aStream);
	/**
	 * (Expected to be changed) Get the storage contents from a stream.
	 * @deprecated by Restore
	 */
	void _restoreFrom(char * fileName);
};

}

#endif//_XMLStorage_
