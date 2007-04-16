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
// Class XMLComponentAdapter
// 

#ifndef _XMLComponentAdapter_h
#define _XMLComponentAdapter_h
#include "BasicXMLable.hxx"
#include "Component.hxx"
#include "Assert.hxx"

namespace CLAM {

/**
 * @ingroup XmlAdapters
 * @brief
 * This class adapts to the XMLable interface any object that is 
 * a CLAM::Component.
 *
 * The XMLComponent adapter implements both the XMLable
 * and the Component interface. So, implements the retrieval of
 * XML properties (naming, being Element, Atribute or Simple 
 * content...) and implements the StoreOn function that is called
 * by the XMLStorage to allow the component store its subitems.
 * The StoreOn is implemented in XMLComponentAdapter simply by
 * forwarding the call to de Adaptee.
 * 
 * You should use XMLComponentAdapter when: 
 * - You have a Component that you don't want always to overload
 *   with the XML stuff, but in some cases you need to store it as
 *   XML content.
 * 
 * Note: Simple content, (XMLable::XMLContent() implementation)
 * returns an empty string.
 * 
 * <B>Pay attention to the management of the name memory</B>
 * (see CLAM::BasicXMLable).
 * 
 * @see XMLable
 * @see Component
 */
class XMLComponentAdapter : public BasicXMLable , public Component {
// Internal Types
public:
	typedef BasicXMLable super;
// Attributes
private:
	Component &myAdaptee;
// Construction/Destruction
public:
	/**
	 * Constructs a XMLComponentAdapter
	 * <p><b>Note:</b> You cannot use this adapter as attribute
	 * because the content adapter does not know how to extract
	 * the XML content. Alternatives: Implement &lt;&lt; and
	 * &gt;&gt; and adapt it with a simple XMLAdapter.
	 * @param adaptee The Component to be adapted (where 
	 * subitem will be located)
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
	 * <b>If a name is given, this must be true because a component
	 * adapter cannot adapt as attribute.</b>
	 */
	template <class T> XMLComponentAdapter (const T & adaptee, const char * name=NULL, bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement), myAdaptee(const_cast<T&>(adaptee))
	{
		CLAM_ASSERT(!name||isXMLElement,"Adapting a component as attribute is useless");
	}
	virtual ~XMLComponentAdapter() {};

// Accessors (for XMLable interface)
public:
	//* @return The XML content: an empty string.
	virtual std::string XMLContent() const
	{
		return "";
	}

	//* Extracts the content from the stream.
	virtual bool XMLContent(std::istream & str) 
	{
		return true;
	}

	//* Returns the class name
	const char * GetClassName() const {return "XMLComponentAdapter";}

// Operators (for Component interface)
public:
	/** 
	 * Stores subitems on the given Storage
	 * @param store The given storage where the subitem will be stored
	 * @see Storage
	 */
	virtual void StoreOn (Storage & store) const {
		myAdaptee.StoreOn(store);
	};
	/**
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be loaded from
	 * @see Storage
	 */
	virtual void LoadFrom (Storage & store) {
		myAdaptee.LoadFrom(store);
	};
// Testing
public:
	//* Check the internal status for a class instance is valid
	bool FulfilsInvariant();
};

}

#endif//_XMLComponentAdapter_h

