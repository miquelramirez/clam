#ifndef _ScopePool_hxx_
#define _ScopePool_hxx_

#include "AttributePool.hxx"
#include "DescriptionScope.hxx"


namespace CLAM
{
	/**
	 * A container for the attributes values along the differents
	 * contexts of a single scope.
	 * @ingroup SemanticalAnalysis
	 */
	class ScopePool : public Component
	{
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
		~ScopePool()
		{
			AttributesData::iterator it = _attributePools.begin();
			AttributesData::iterator end = _attributePools.end();
			for (; it!=end; it++) it->Deallocate();
			_size=0;
		}
		const char * GetClassName() const { return "ScopePool"; }
		void StoreOn(Storage & storage) const
		{
			XMLAdapter<std::string> nameAdapter(_spec.GetName(),"name",false);
			storage.Store(nameAdapter);
			XMLAdapter<unsigned> sizeAdapter(_size,"size",false);
			storage.Store(sizeAdapter);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
			{
				if (_size && !_attributePools[attribute].GetData()) continue;
				XMLComponentAdapter adapter(_attributePools[attribute],"AttributePool",true);
				storage.Store(adapter);
			}
		}
		void LoadFrom(Storage & storage)
		{
			std::string name;
			XMLAdapter<std::string> nameAdapter(name,"name",false);
			storage.Load(nameAdapter);
			CLAM_ASSERT(name==_spec.GetName(),"Loading an scope pool for a different attribute");

			unsigned newSize;
			XMLAdapter<unsigned> sizeAdapter(newSize,"size",false);
			storage.Load(sizeAdapter);
			Reallocate(newSize);
			for (unsigned attribute=0; attribute<_attributePools.size(); attribute++)
			{
				_attributePools[attribute].Deallocate();
				_attributePools[attribute].Allocate(_size);
//				if (_size && !_attributePools[attribute].GetData()) continue;
				XMLComponentAdapter adapter(_attributePools[attribute],"AttributePool",true);
				storage.Load(adapter);
			}
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
				if (newSize) it->Allocate(_size);
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

		template <typename AttributeType>
		const AttributeType * GetReadPool(const std::string & name) const
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			CLAM_ASSERT(_attributePools[attribPos].GetData(),
				(std::string()+"Getting data from a non instanciated attribute '"+_spec.GetName()+"':'"+name+"'").c_str());
			return (const AttributeType*) _attributePools[attribPos].GetData();
		}

		template <typename AttributeType>
		AttributeType * GetWritePool(const std::string & name)
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			if (!_attributePools[attribPos].GetData())
				_attributePools[attribPos].Allocate(_size);
			return (AttributeType*) _attributePools[attribPos].GetData();
		}
	};

}


#endif// _ScopePool_hxx_

