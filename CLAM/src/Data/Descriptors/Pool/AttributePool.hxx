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
	private:
		void * _data;
		const AbstractAttribute * _attribute;
		unsigned _size;
	};
}


#endif// _AttributePool_hxx_

