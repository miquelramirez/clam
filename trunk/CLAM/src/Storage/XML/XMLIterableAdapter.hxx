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
// Class XMLIterableAdapter
// 

#ifndef _XMLIterableAdapter_h
#define _XMLIterableAdapter_h

#include "BasicXMLable.hxx"
#include "XMLAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "TypeInfo.hxx"
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
template <class T> class XMLIterableAdapter : public BasicXMLable , public Component {
// Internal Types
public:
	typedef BasicXMLable super;
	typedef T t_adaptee;
	typedef typename t_adaptee::value_type t_adapteeValues;
	typedef typename t_adaptee::iterator t_adapteeIterator;
// Attributes
private:
	t_adaptee & mAdaptee;
	const char * mElementsName;
// Construction/Destruction
public:
	/**
	 * Constructs a XMLIterableAdapter
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
	XMLIterableAdapter (T & anAdaptee, const char * elementName, const char * name=NULL, 
			bool isXMLElement=false)
		: BasicXMLable(name, isXMLElement), mAdaptee(anAdaptee)
	{
		mElementsName = elementName;
	}
	virtual ~XMLIterableAdapter() {};

// Accessors
public:
	//* @return A string with the extracted XML content
	std::string XMLContent() 
	{
		return ContentLeaveOrComponent((TypeInfo<t_adapteeValues>::StorableAsLeaf*)NULL);
	}

	//* Extracts the content from the stream.
	bool XMLContent(std::istream & str) 
	{
		return ContentLeaveOrComponent((TypeInfo<t_adapteeValues>::StorableAsLeaf*)NULL, str);
	}

// Operators (for Component interface)
public:
	/** 
	 * Stores subitems on the given Storage
	 * @param store The given storage where the subitem will be stored
	 * @see Storage
	 */
	virtual void StoreOn (Storage & store){
		t_adapteeIterator it=mAdaptee.begin();
		t_adapteeIterator end=mAdaptee.end();
		for (; it!=end; it++) {
			StoreLeaveOrComponent (store, *it, mElementsName,
			                       (TypeInfo<t_adapteeValues>::StorableAsLeaf*)NULL
			                       );
		}
	};
	/**
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be loaded from
	 * @todo Not yet implemented
	 * @see Storage
	 */
	virtual void LoadFrom (Storage & store) {
		mAdaptee.clear();
		while (true) {
			t_adapteeValues elem;
			if (!LoadLeaveOrComponent(store, elem, mElementsName,
						(TypeInfo<t_adapteeValues>::StorableAsLeaf *)NULL)
					)
				break;
			mAdaptee.push_back(elem);
		}
	};
// Implementation:
private:
	void StoreLeaveOrComponent (Storage& store, t_adapteeValues & value, const char * mElemName, StaticTrue* isLeave) {
		XMLAdapter<t_adapteeValues> adapter(value);
		store.Store(&adapter);
	}
	void StoreLeaveOrComponent (Storage& store, t_adapteeValues & value, const char * mElemName, StaticFalse* isLeave) {
		XMLComponentAdapter adapter(value,mElemName,true);
		store.Store(&adapter);
	}
	bool LoadLeaveOrComponent (Storage& store, t_adapteeValues & value, const char * mElemName, StaticTrue* isLeave) {
		XMLAdapter<t_adapteeValues> adapter(value);
		return store.Load(&adapter);
	}
	bool LoadLeaveOrComponent (Storage& store, t_adapteeValues & value, const char * mElemName, StaticFalse* isLeave) {
		XMLComponentAdapter adapter(value,mElemName,true);
		return store.Load(&adapter);
	}
	//* @return A string with the extracted XML content
	std::string ContentLeaveOrComponent(StaticFalse* isLeave) 
	{
		return "";
	}
	/// @return A string with the extracted XML content
	std::string ContentLeaveOrComponent(StaticTrue* isLeave) 
	{
		if (!IsXMLAttribute()) return "";
		std::stringstream stream;
		t_adapteeIterator it=mAdaptee.begin();
		t_adapteeIterator end=mAdaptee.end();
		bool first = true;
		for (; it!=end; it++) {
			if (first) first=false;
			else stream << " ";
			stream << *it;
		}
		return stream.str();
	}
	/**
	 * @todo Not yet implemented
	 */
	bool ContentLeaveOrComponent(StaticFalse* isLeave, std::istream &is) 
	{
		return true;
	}
	/**
	 * @todo Not yet implemented
	 */
	bool ContentLeaveOrComponent(StaticTrue* isLeave, std::istream &str) 
	{
		if (!IsXMLAttribute()) return true;
		mAdaptee.clear();
		while (str) {
			t_adapteeValues contained;
			str >> contained;
			if (str) mAdaptee.push_back(contained);
		}
		return true;
	}
// Testing
public:
	/// Check the internal status for a class instance is valid
	bool FulfilsInvariant();
};


// Check the internal status for a class instance is valid
template <class T>
bool XMLIterableAdapter<T>::FulfilsInvariant() 
{
	return super::FulfilsInvariant();
};

}

#endif//_XMLIterableAdapter_h
