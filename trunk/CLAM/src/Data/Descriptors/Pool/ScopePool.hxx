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

#ifndef _ScopePool_hxx_
#define _ScopePool_hxx_

#include "AttributePool.hxx"
#include "DescriptionScope.hxx"

// TODO TO-TEST:
// Reading an non existent attribute
// Not reading an existent attribute
// Unordered attributes
// Twice present attributes

namespace CLAM
{
	/**
	 * A container for the attributes values along the differents
	 * contexts of a single scope.
	 * @ingroup SemanticalAnalysis
	 */
	class ScopePool : public Component
	{
		friend class AttributePoolStorageProxy;
		class AttributePoolStorageProxy : public Component
		{
				ScopePool & pool;
			public:
				AttributePoolStorageProxy(ScopePool & scopePool)
					: pool(scopePool)
				{
				}
				const char* GetClassName() const { return "AttributeProxy"; }
				void StoreOn(CLAM::Storage&) const {}
				void LoadFrom(Storage & storage)
				{
					std::string name;
					XMLAdapter<std::string> nameAdapter(name,"name",false);
					storage.Load(nameAdapter);
					unsigned attributeIndex = pool._spec.GetIndexSafe(name);
					pool._attributePools[attributeIndex].Allocate(pool._size);
					XMLComponentAdapter adapter(pool._attributePools[attributeIndex]);
					storage.Load(adapter);
				}
		};
	public:
		typedef std::vector<AttributePool> AttributesData;
	private:
		unsigned _size;
		const DescriptionScope & _spec;
		AttributesData _attributePools;
	public:
		ScopePool(const DescriptionScope & spec, unsigned size=0)
			: _size(size), _spec(spec), _attributePools(spec.GetNAttributes())
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				it->SetDefinition(_spec.GetAttribute(i));
			}
		}
		~ScopePool();
		const char * GetClassName() const { return "ScopePool"; }
		void StoreOn(Storage & storage) const
		{
			XMLAdapter<std::string> nameAdapter(_spec.GetName(),"name",false);
			storage.Store(nameAdapter);
			XMLAdapter<unsigned> sizeAdapter(_size,"size",false);
			storage.Store(sizeAdapter);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
			{
				if (!_attributePools[attribute].GetData()) continue;
				XMLComponentAdapter adapter(_attributePools[attribute],"AttributePool",true);
				storage.Store(adapter);
			}
		}
		void LoadFrom(Storage & storage)
		{
			std::string name;
			XMLAdapter<std::string> nameAdapter(name,"name",false);
			storage.Load(nameAdapter);
			CLAM_ASSERT(name==_spec.GetName(),
				("The schema expected a scope named '"+_spec.GetName()+
				 "', but the XML contains the scope '"+ name+"' instead").c_str());
			unsigned newSize;
			XMLAdapter<unsigned> sizeAdapter(newSize,"size",false);
			storage.Load(sizeAdapter);
			Reallocate(newSize);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
				_attributePools[attribute].Deallocate();
			AttributePoolStorageProxy proxy(*this);
			XMLComponentAdapter adapter(proxy,"AttributePool",true);
			while (storage.Load(adapter)) {} // do nothing
		}
		void Insert(unsigned pos)
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				it->Insert(pos);
			}
			_size++;
		}
		void Remove(unsigned pos)
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				it->Remove(pos);
			}
			_size--;
		}
	private:
		void Reallocate(unsigned newSize)
		{
			_size = newSize;
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (unsigned i=0; it!=end; i++, it++)
			{
				if (!it->GetData()) continue;
				it->Deallocate();
				it->Allocate(_size);
			}
		}
	public:
		unsigned GetNAttributes() const
		{
			return _spec.GetNAttributes();
		}
		unsigned GetSize() const
		{
			return _size;
		}
		void SetSize(unsigned newSize)
		{
			Reallocate(newSize);
		}

		bool IsInstantiated(const std::string & attributeName) const
		{
			unsigned attribPos = _spec.GetIndex(attributeName);
			const void * data = _attributePools[attribPos].GetData();
			return data!=0;
		}
		template <typename AttributeType>
		const AttributeType * GetReadPool(const std::string & name) const
		{
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			const void * data = _attributePools[attribPos].GetData();
			CLAM_ASSERT(data,
				(std::string()+"Getting data from a non instanciated attribute '"+_spec.GetName()+"':'"+name+"'").c_str());
			return &(*(const std::vector<AttributeType>*) data )[0];
		}

		template <typename AttributeType>
		AttributeType * GetWritePool(const std::string & name)
		{
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			void * data = _attributePools[attribPos].GetData();
			if (!data)
			{
				_attributePools[attribPos].Allocate(_size);
				data = _attributePools[attribPos].GetData();
			}

			return &(*(std::vector<AttributeType>*) data )[0];
		}
	};

}


#endif// _ScopePool_hxx_

