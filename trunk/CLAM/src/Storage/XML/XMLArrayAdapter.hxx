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
// Class XMLArrayAdapter
// 

#ifndef _XMLArrayAdapter_h
#define _XMLArrayAdapter_h

#include "BasicXMLable.hxx"
#include "mtgsstream.h" // An alias for <sstream>

namespace CLAM {

/**
 * This class adapts to the XMLable interface any array of objects 
 * which has the insertion (&lt;&lt;) operator defined to an ostream, 
 * and calculates the XML content with such operator on storage time.
 * The content is the same one which the insertion operator 
 * generates inserting an space between elements.
 * 
 * <P>During the construction, the adapter stores a reference to
 * the adaptee. The content string extracted on demand when the
 * adapter is stored on a XMLStorage, so the adapter is sensitive
 * to the adaptee changes after the construction. 
 *
 * <P><B>Important:</B> At storage time, the adaptee must exist in
 * order to follow the reference.
 *
 * <P><B>Pay attention to the management of the name memory</B>
 * (@see BasicXMLable).
 * 
 * @see XMLable
 * @see XMLAdapter
 * @see XMLStaticAdapter
 * @see XMLComponentAdapter
 */
template <class T> class XMLArrayAdapter : public BasicXMLable {
// Internal Types
public:
	typedef BasicXMLable super;
	typedef T t_adaptee;
// Attributes
private:
	t_adaptee * mAdaptee;
	unsigned int size;
// Construction/Destruction
public:
	/**
	 * Constructs a XMLArrayAdapter
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
	XMLArrayAdapter (t_adaptee * anAdaptee, unsigned int nElements, const char * name=NULL, bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement), mAdaptee(anAdaptee)
	{
		size = nElements;
	}
	virtual ~XMLArrayAdapter() {};

// Accessors
public:
	//* @return A string with the extracted XML content
	std::string XMLContent() 
	{
		std::string s;
		s.resize(size*15);
		std::stringstream str(s);
		for (unsigned int i=0; i<size; i++) {
			str << mAdaptee[i];
			if (i != size-1) str << " ";
		}
		str << std::ends;
		return str.str();
	}

	//* Extracts the content from the stream.
	bool XMLContent(std::istream & str) 
	{
		for (unsigned int i=0; i<size; i++) {
			str >> mAdaptee[i];
		}
		return bool(str.good());
	}

// Testing
public:
	//* Check the internal status for a class instance is valid
	bool FulfilsInvariant() {
		return super::FulfilsInvariant();
	}
};
}


#endif//_XMLArrayAdapter_h
