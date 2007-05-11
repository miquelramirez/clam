/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _DescriptionScope_hxx_
#define _DescriptionScope_hxx_

#include "DescriptionAttributes.hxx"
#include <map>
#include <vector>

namespace CLAM
{
	/**
	 * A description scope defines a set of attributes which have 
	 * the same ocurrences.
	 * For example,
	 * some attributes will have one occurrence by each note, 
	 * while some others will have one occurrence by each frame.
	 * Thus, we talk about Frame scope, Note scope...
	 *
	 * At least that you were modifying the DescriptionScheme
	 * and DescriptionDataPool implementations, or something related,
	 * you shouldn't use this class directly, use DescriptionScheme instead.
	 * @see SemanticalAnalysis module, for more information on that.
	 * @ingroup SemanticalAnalysis
	 */
	class SchemaError : public Err
	{
		public:
		SchemaError(const std::string & msg) : Err(msg.c_str()) {}
	};

	class DescriptionScope
	{
	public:
		typedef std::map<std::string, unsigned> NamesMap;
		typedef std::vector<AbstractAttribute *> Attributes;
	private:
		NamesMap _nameMap;
		Attributes _attributes;
		std::string _scopeName;
	public:
		DescriptionScope(const std::string & name) : _scopeName(name) {}
		~DescriptionScope();

		/** @return the scope name */
		const std::string & GetName() const
		{
			return _scopeName;
		}

		/** 
		 * Adds the scope an attribute named 'name' using the template parameter 
		 * as the attribute type.
		 * @arg AttributeType The type for the attribute
		 * @arg name The name for the new attribute
		 */
		template <typename AttributeType>
		void Add(const std::string & name)
		{
			unsigned attributeIndex = _nameMap.size();
			bool inserted = 
				_nameMap.insert(std::make_pair(name,attributeIndex)).second;
			CLAM_ASSERT(inserted,("Couldn't add attribute '"+_scopeName+":"+name+"', already present in the scope").c_str());
			_attributes.push_back(new Attribute<AttributeType>(_scopeName, name));
		}

		/**
		 * Returns the attribute index that can be used for fast access to the 
		 * attribute in a spec or even . 
		 * @warning The index is not a confident reference after serialization.
		 * @pre the name should exists
		 */
		unsigned GetIndex(const std::string & name) const
		{
			NamesMap::const_iterator it = _nameMap.find(name);
			CLAM_ASSERT(it!=_nameMap.end(),
				(std::string()+"Accessing an unexisting attribute '"+_scopeName+"':'"+name+"'").c_str());
			return it->second;
		}
		/**
		 * The same as GetIndex but it throws a catchable exception whenever
		 * the name is not available
		 */
		unsigned GetIndexSafe(const std::string & name) const
		{
			NamesMap::const_iterator it = _nameMap.find(name);
			if (it==_nameMap.end())
				throw SchemaError("Accessing an unexisting attribute '"+_scopeName+"':'"+name+"'");
			return it->second;
		}

		unsigned GetNAttributes() const
		{
			return _nameMap.size();
		}

		void * Allocate(unsigned attribute, unsigned size) const
		{
			return _attributes[attribute]->Allocate(size);
		}
		void Deallocate(unsigned attribute, void * buffer) const
		{
			_attributes[attribute]->Deallocate(buffer);
		}

		template <typename AttributeType>
		void CheckType(unsigned attributeIndex, AttributeType *) const
		{
			_attributes[attributeIndex]->CheckType<AttributeType>();
		}

		const std::string & GetAttributeName(unsigned attributeIndex) const
		{
			CLAM_ASSERT(attributeIndex<_attributes.size(),
				"GetAttributeName: Using a wrong index to look up an attribute name");
			AbstractAttribute * attribute = _attributes[attributeIndex];
			return attribute->GetName();
		}
		const AbstractAttribute & GetAttribute(unsigned int attribute) const
		{
			return * _attributes[attribute];
		}
	};

}


#endif// _DescriptionScope_hxx_

