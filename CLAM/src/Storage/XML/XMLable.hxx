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

///////////////////////////////////////////////////////////////////////////////
// Class XMLable: Interface for objects to be stored on a XMLStorage
///////////////////////////////////////////////////////////////////////////////
// Description: 
// Abstract class that defines the interface a object needs to be stored on
// a XMLStorage. An XML storage will need the information offered by those
// methods in order to know the XML structure.
///////////////////////////////////////////////////////////////////////////////

#ifndef _XMLable_
#define _XMLable_

#include "Storable.hxx"
#include <string>

namespace CLAM {

/**
 * Interface for objects to be stored on a XMLStorage.
 * Abstract class that defines the interface a object needs to be stored on
 * a XMLStorage. An XML storage will need the information offered by those
 * methods in order to know the XML structure.
 */

class XMLable : public Storable {
// Construction/Destruction
public:
	virtual ~XMLable();
// Redifinible methods
public:
	//* @return A pointer to the XMLName
	virtual const char * XMLName() const =0;
	//* @return A string with the extracted XML content
	virtual std::string XMLContent() const =0;
	//* @TODO Implement this
	virtual bool XMLContent(std::istream & contentHolder)=0;
	//* @return Whether the object represents an XML attribute or not
	virtual bool IsXMLAttribute() const =0;
	//* @return Whether the object represents an XML element or not
	virtual bool IsXMLElement() const =0;
	//* @return Whether the object represents an XML text or not
	virtual bool IsXMLText() const =0;
};

}

#endif//_XMLable_

