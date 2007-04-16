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
// Class XMLArrayAdapter
// 

#ifndef _XMLArrayAdapter_h
#define _XMLArrayAdapter_h

#include "BasicXMLable.hxx"
#include <sstream>

namespace CLAM {

/**
 * @ingroup XmlAdapters
 * @brief This class adapts to the XMLable interface any array of basic objects.
 *
 * A basic object is that one that has
 * the insertion (<<) and extractor (>>) operators defined to streams
 * and there exists a TypeInfo struct for it (see CLAM_TYPE_INFO_GROUP).
 * The adapter uses such operators to calculate the XML content on writing
 * and reconstruct the object on reading.
 * 
 * During the construction, the adapter stores a reference to
 * the adaptee. The content string extracted on demand when the
 * adapter is stored on a XMLStorage, so the adapter is sensitive
 * to the adaptee changes after the construction. 
 *
 * <B>Important:</B> At storage time, the adaptee must exist in
 * order to follow the reference.
 *
 * <B>Pay attention to the management of the name memory</B>
 * (see BasicXMLable).
 * 
 * @see XMLable
 * @see XMLAdapter
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
	 * @param anAdaptee The object to be adapted (where the 
	 * XML content will be extracted from)
	 * @param name A pointer to a 0 terminated string 
	 * containing the xml name (for elements and attributes)
	 * or 0 (the default) if the element is neither an 
	 * element nor an attribute (@see #BasicXMLable for
	 * important details about memory managing).
	 * <B>Because no internal copy of the string is done, the 
	 * 0 terminated string pointed by <EM>name</EM> must 
	 * exist during the BasicXMLable life as is directly used.</B>
	 * @param nElements The number of elements on the array.
	 * @param isXMLElement Tells whether the object is an
	 * element or an attribute when the name is defined.
	 */
	XMLArrayAdapter (t_adaptee * anAdaptee, unsigned int nElements, const char * name=NULL, bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement), mAdaptee(anAdaptee)
	{
		size = nElements;
	}
	XMLArrayAdapter (const t_adaptee * anAdaptee, unsigned int nElements, 
			const char * name=NULL, bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement), mAdaptee(const_cast<T*>(anAdaptee))
	{
		size = nElements;
	}
	virtual ~XMLArrayAdapter() {};

// Accessors
public:
	//* @return A string with the extracted XML content
	std::string XMLContent() const
	{
		std::string s;
		s.resize(size*15);
		std::stringstream str(s);
		for (unsigned int i=0; i<size; i++) {
			str << mAdaptee[i];
			if (i < size-1) str << " ";
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

