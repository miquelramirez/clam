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
// Class XMLStaticAdapter
// 

#ifndef _XMLStaticAdapter_
#define _XMLStaticAdapter_

#include "BasicXMLable.hxx"
#include "mtgsstream.h" // An alias for <sstream>

namespace CLAM {

/**
 * This class adapts to the XMLable interface any object that has 
 * the insertion (&lt;&lt;) operator defined to an ostream, and
 * calculates the XML content with such operator on construction time.
 * The content is the same one which the insertion operator 
 * generates.
 * 
 * <P>During the construction of the adapter, the content string 
 * extracted with the insertion operator is stored statically
 * in the adapter. So, the generated XML content is unsensible to
 * any changes done after the construction.
 * 
 * <p><b>Note:</b> XMLStaticAdapter are usable only for
 * storing, not for loading, because they keep no
 * reference to the adaptee, the can't give it the content.
 *
 * <P><B>Pay attention to the management of the name memory</B>
 * (@see BasicXMLable).
 * 
 * <P>You should use XMLStaticAdapter instead a normal XMLAdapter
 * when: 
 * <UL>
 * <LI>The adaptee is going to vary and you want to store the state 
 * at a concrete moment.
 * <LI>The adaptee will be not available at storage time.
 * <LI>The same adaptee is stored a lot of times and you want to
 * reuse the same extraction all the time.
 * <LI>You hate templatized types.
 * </UL>
 * 
 * <P>A normal XMLAdapter is more useful than an XMLStaticAdapter
 * specially when:
 * <UL>
 * <LI>You want to follow the changes on the adaptee.
 * <LI>The Adapter is going to be permanently in memory and you don't
 * want to expend extra memory for the content container.
 * </UL>
 * 
 * @see XMLable
 * @see XMLAdapter
 */
class XMLStaticAdapter : public BasicXMLable {
// Internal Types
public:
	typedef BasicXMLable super;
// Attributes
private:
	std::string mContent;
// Construction/Destruction
public:
	/**
	 * Constructs a XMLStaticAdapter.
	 * @param adaptee The object to be adapted (where the 
	 * XML content will be extracted from)
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
	template <class T> XMLStaticAdapter (const T & adaptee, const char * name=NULL, bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement)
	{
		std::stringstream str;
		str << adaptee << std::ends;

		mContent = str.str();
	}
	virtual ~XMLStaticAdapter() {};

// Accessors
public:
	//* @return A string with the extracted XML content
	virtual std::string XMLContent() const
	{
		return mContent;
	}

	//* This function is not to be used! Extracts the content from the stream.
	bool XMLContent(std::istream & str) 
	{
		CLAM_ASSERT (false, "An StaticAdapter can't be used for loading");
		return false;
	}
// Testing
public:
	//* Check the internal status for a class instance is valid
	bool FulfilsInvariant();
};

}

#endif//_XMLStaticAdapter_
