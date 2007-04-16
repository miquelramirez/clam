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
// Class BasicXMLable
// Abstract class that defines some common implementation issues for
// many XMLables that contains as member variables the XML name and
// whether is an XML element or not.

#ifndef _BasicXMLable_h
#define _BasicXMLable_h

#include "XMLable.hxx"
#include "Assert.hxx"
#include <ctype.h>
#include <string>

namespace CLAM {

/**
 * Abstract class that defines some common implementation issues for
 * many XMLables that contains as member variables the XML name and
 * whether is an XML element or not.
 * <P>
 * If you don't need those implementation issues consider using 
 * XMLable instead which defines only the interface.
 */
class BasicXMLable : public XMLable {
// Attributes
private:
	const char * myXMLName;
	bool amIXMLElement;
// Construction/Destruction
public:
	/**
	 * Constructs a BasicXMLable
	 * @param name A pointer to a 0 terminated string 
	 * containing the xml name (for elements and attributes)
	 * or 0 (the default) if the element is neither an 
	 * element nor an attribute (@see #BasicXMLable for
	 * important details about memory managing).
	 * <B>Because no internal copy of the string is done, the 
	 * 0 terminated string pointed by <EM>name</EM> must 
	 * exist during the BasicXMLable life as is directly used.</B>
	 * @param isXMLElement Tells whether the object is an
	 * element or an attribute when the name is defined.
	 */
	BasicXMLable (const char * name=0, bool isXMLElement=false);
	
	virtual ~BasicXMLable() {};
// Accessors
public:
	//* @return A pointer to the XMLName
	const char * XMLName() const
	{
		return myXMLName;
	}
	//* @return Whether the object represents an XML element or not
	bool IsXMLElement() const
	{
		return myXMLName && amIXMLElement;
	}
	//* @return Whether the object represents an XML attribute or not
	bool IsXMLAttribute() const
	{
		return myXMLName && !amIXMLElement;
	}
	bool IsXMLText() const
	{
		return myXMLName==0;
	}
	//* @return A string with the XML content
	virtual std::string XMLContent() const=0;
// Testing
public:
	//* Check the internal status for a class instance is valid
	bool FulfilsInvariant();
};

}

#endif//_BasicXMLable_h

