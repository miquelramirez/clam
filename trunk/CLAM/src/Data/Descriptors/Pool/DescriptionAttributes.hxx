#ifndef _DescriptionAttributes_hxx_
#define _DescriptionAttributes_hxx_

#include <typeinfo>
#include "Assert.hxx"
#include "Storage.hxx"
#include "XMLAdapter.hxx"
#include "XMLArrayAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "Component.hxx"

namespace CLAM
{
	/**
	 * Defines the interface for an Attribute definition in a DescriptionScheme.
	 * It offers a type independent interface to do type dependent operations
	 * defined on the specialization of the Attribute template.
	 * @ingroup SemanticalAnalysis 
	 */
	class AbstractAttribute
	{
	public:
		AbstractAttribute(const std::string & attributeName) : _attributeName(attributeName) {}
		virtual ~AbstractAttribute() {}
		/** Returns the attribute name */
		const std::string & GetName() const { return _attributeName; }
		
		/** Allocates and construct 'size' elements for the attribute */
		virtual void * Allocate(unsigned size) const = 0;
		/** Destroys and deallocates the elements pointed by 'data' */
		virtual void Deallocate(void * data) const = 0;

		/** Dumps the 'data' into the storage */
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size ) const = 0;
		/** Restore the 'data' from the storage */
		virtual void XmlRestoreData(Storage & storage, void * data, unsigned size ) const = 0;

		/** Asserts false whenever TypeToCheck is not the one that the attribute contains */
		template <typename TypeToCheck>
		void CheckType() const
		{
			CLAM_ASSERT(typeid(TypeToCheck)==TypeInfo(),
				"Type Missmatch using a pool");
		}
	protected:
		/** Returns the type_info for the attribute type */
		virtual const std::type_info & TypeInfo() const = 0;
	private:
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
		Attribute(const std::string & attributeName) : AbstractAttribute(attributeName) {}
		typedef AttributeType DataType;
		virtual void * Allocate(unsigned size) const
		{
			return new AttributeType[size];
		}
		virtual void Deallocate(void * data) const
		{
			delete [] (AttributeType*)data;
		}
		virtual void XmlDumpData(Storage & storage, const void * data, unsigned size ) const
		{
			XMLAdapter<std::string> nameAdapter(GetName(),"name",false);
			storage.Store(nameAdapter);
			XmlDumpConcreteData(storage,(AttributeType*)data,size,(AttributeType*)0);
		}
		virtual void XmlRestoreData(Storage & storage, void * data, unsigned size ) const
		{
			std::string name;
			XMLAdapter<std::string> nameAdapter(name,"name",false);
			storage.Load(nameAdapter);
			CLAM_ASSERT(name==GetName(),"Loading a attribute pool for a different attribute");
			XmlRestoreConcreteData(storage,(AttributeType*)data,size,(AttributeType*)0);
		}
	private:
		template <typename T>
		void XmlDumpConcreteData(Storage & storage, const T * data, unsigned size, void * discriminator ) const
		{
			XMLArrayAdapter<AttributeType> dataAdapter((AttributeType*)data, size);
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
			XMLArrayAdapter<AttributeType> dataAdapter(data, size);
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
			return typeid(AttributeType);
		}
	};

}



#endif// _DescriptionAttributes_hxx_
