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

#ifndef _AttributePool_hxx_
#define _AttributePool_hxx_

#include "DescriptionAttributes.hxx"

namespace CLAM
{
	/**
	 * A container for the values for a single attribute along the scope.
	 * @ingroup SemanticalAnalysis
	 */
	class AttributePool : public Component
	{
	public:
		AttributePool()
		{
			_data=0;
			_size=0;
			_attribute=0;
		}
		void SetDefinition(const AbstractAttribute & attribute)
		{
			_attribute = & attribute;
		}
		const char * GetClassName() const { return "AttributePool"; }
		void StoreOn(Storage & storage) const
		{
			_attribute->XmlDumpData(storage, _data, _size);
		}
		void LoadFrom(Storage & storage)
		{
			_attribute->XmlRestoreData(storage, _data, _size);
		}
		void * GetData() { return _data; }
		const void * GetData() const { return _data; }
		void Allocate(unsigned size)
		{
			_data = _attribute->Allocate(size);
			_size=size;
		}
		void Deallocate()
		{
			if (!_data) return;
			_attribute->Deallocate(_data);
			_data = 0;
		}
		void Insert(unsigned pos)
		{
			_size++;
			if (_data)
				_attribute->Insert(_data,pos);
		}
		void Remove(unsigned pos)
		{
			_size--;
			if (_data)
				_attribute->Remove(_data,pos);
		}
	private:
		void * _data;
		const AbstractAttribute * _attribute;
		unsigned _size;
	};
}


#endif// _AttributePool_hxx_

