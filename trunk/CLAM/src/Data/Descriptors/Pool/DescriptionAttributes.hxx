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

#ifndef _DescriptionAttributes_hxx_
#define _DescriptionAttributes_hxx_

#include <typeinfo>
#include "Assert.hxx"
#include "Storage.hxx"
#include "XMLAdapter.hxx"
#include "XMLArrayAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "Component.hxx"
#include <typeinfo>
#include <vector>

namespace CLAM
{
	/**
	 * Defines the interface for an Attribute definition in a DescriptionScheme.
	 * It offers a type independent interface to do type dependent operations
	 * defined on the specialization of the Attribute template.
	 * @ingroup SemanticalAnalysis 
	 */
	class AbstractAttribute : public Component
	{
	public:
		AbstractAttribute(const std::string & attributeName)
			: _attributeName(attributeName) {}
		AbstractAttribute(const std::string & scopeName, const std::string & attributeName)
			: _scopeName(scopeName), _attributeName(attributeName) {}
		virtual ~AbstractAttribute();

		const char * GetClassName() const { return "AbstractAttribute"; }
		void StoreOn(Storage & storage) const
		{
			XMLAdapter<std::string> scopeAttribute(_scopeName, "scope", false);
			storage.Store(scopeAttribute);

			XMLAdapter<std::string> nameAttribute(_attributeName, "name", false);
			storage.Store(nameAttribute);

			XmlDumpSchemaType(storage);
		}
		void LoadFrom(Storage & storage)
		{
		}
		
		/** Returns the attribute name */
		const std::string & GetName() const { return _attributeName; }
		/** Returns the attribute name */
		const std::string & GetScope() const { return _scopeName; }
		
		/** Allocates and construct 'size' elements for the attribute */
		virtual void * Allocate(unsigned size) const = 0;
		/** Destroys and deallocates the elements pointed by 'data' */
		virtual void Deallocate(void * data) const = 0;
		/** Inserts am element at position pos */
		virtual void Insert(void * data, unsigned pos) const = 0;
		/** Removes an element at position pos */
		virtual void Remove(void * data, unsigned pos) const = 0;

		/** Dumps the 'data' into the storage */
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size ) const = 0;
		/** Restore the 'data' from the storage */
		virtual void XmlRestoreData(Storage & storage, void * data, unsigned size ) const = 0;

		/** Dumps the type definition for the attribute into the storage */
		virtual void XmlDumpSchemaType(Storage & storage) const = 0;

		/** Asserts false whenever TypeToCheck is not the one that the attribute contains */
		template <typename TypeToCheck>
		void CheckType() const
		{
			if (typeid(TypeToCheck)==TypeInfo()) return;
			std::ostringstream os;
			os << "Attribute '" << _scopeName << ":" << _attributeName 
				<< "' has been used as type '" << typeid(TypeToCheck).name()
				<< "' but it really was of type '" << TypeInfo().name() << "'";
			CLAM_ASSERT(typeid(TypeToCheck)==TypeInfo(),
				os.str().c_str());
		}
	protected:
		/** Returns the type_info for the attribute type */
		virtual const std::type_info & TypeInfo() const = 0;
	private:
		std::string _scopeName;
		std::string _attributeName;
	};

	/**
	 * This class is the concrete implementation for AbstractAttribute for a given type of attributes.
	 * It implements functionalities that are type dependant in an attribute.
	 * @ingroup SemanticalAnalysis
	 */
	template <typename AttributeType>
	class Attribute : public AbstractAttribute
	{
	public:
		Attribute(const std::string & attributeName)
			: AbstractAttribute(attributeName) {}
		Attribute(const std::string & scopeName, const std::string & attributeName)
			: AbstractAttribute(scopeName, attributeName) {}
		typedef AttributeType DataType;
		virtual void * Allocate(unsigned size) const
		{
			return new std::vector<DataType>(size);
		}
		virtual void Deallocate(void * data) const
		{
			delete (std::vector<DataType>*)data;
		}
		virtual void Insert(void * data, unsigned pos) const
		{
			std::vector<DataType> * vector = (std::vector<DataType> *) data;
			vector->insert(vector->begin()+pos, DataType());
		}
		virtual void Remove(void * data, unsigned pos) const
		{
			std::vector<DataType> * vector = (std::vector<DataType> *) data;
			vector->erase(vector->begin()+pos);
		}
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size ) const
		{
			XMLAdapter<std::string> nameAdapter(GetName(),"name",false);
			storage.Store(nameAdapter);
			const std::vector<DataType> * vector = (const std::vector<DataType> *) data;
			XmlDumpConcreteData(storage,&(*vector)[0],size,(DataType*)0);
		}
		virtual void XmlRestoreData(Storage & storage, void * data, unsigned size ) const
		{
			std::string name;
			XMLAdapter<std::string> nameAdapter(name,"name",false);
			storage.Load(nameAdapter);
			CLAM_ASSERT(name==GetName(), 
				("The schema expected an attribute named '" + GetScope() + ":" + GetName() + 
				 "' but the XML file contained '" + GetScope() + ":" + name + "'").c_str());
			std::vector<DataType> * vector = (std::vector<DataType> *) data;
			XmlRestoreConcreteData(storage,&(*vector)[0],size,(DataType*)0);
		}
		void XmlDumpSchemaType(Storage & storage) const
		{
			DataType * typeDiscriminator = 0;
			std::string type = XmlTypeId(typeDiscriminator, typeDiscriminator);
			XMLAdapter<std::string> typeAttribute(type, "type", false);
			storage.Store(typeAttribute);
		}
	private:
		template <typename T>
		std::string XmlTypeId(const T * realType, void * discriminator ) const
		{
		//	return "Unknown";
			return typeid(T).name();
		}
		template <typename T>
		std::string XmlTypeId(const T * realType, Component * discriminator ) const
		{
			T dummy;
			return dummy.GetClassName();
		}
		template <typename T>
		std::string XmlTypeId(const T * realType, float * discriminator ) const
		{
			return "Float";
		}
		template <typename T>
		std::string XmlTypeId(const T * realType, std::string * discriminator ) const
		{
			return "String";
		}
		template <typename T>
		void XmlDumpConcreteData(Storage & storage, const T * data, unsigned size, void * discriminator ) const
		{
			XMLArrayAdapter<DataType> dataAdapter((DataType*)data, size);
			storage.Store(dataAdapter);
		}
		template <typename T>
		void XmlDumpConcreteData(Storage & storage, const T * data, unsigned size, Component * discriminator ) const
		{
			for (unsigned i=0 ; i < size ; i++ )
			{
				XMLComponentAdapter componentAdapter(data[i],data[i].GetClassName(),true);
				storage.Store(componentAdapter);
			}
		}
		template <typename T>
		void XmlRestoreConcreteData(Storage & storage, T * data, unsigned size, void * discriminator ) const
		{
			XMLArrayAdapter<DataType> dataAdapter(data, size);
			storage.Load(dataAdapter);
		}
		template <typename T>
		void XmlRestoreConcreteData(Storage & storage, T * data, unsigned size, Component * discriminator ) const
		{
			for (unsigned i=0 ; i < size ; i++ )
			{
				XMLComponentAdapter componentAdapter(data[i],data[i].GetClassName(),true);
				storage.Load(componentAdapter);
			}
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(DataType);
		}
	};

}



#endif// _DescriptionAttributes_hxx_

