#include <string>
#include <map>
#include <typeinfo>

#include "Assert.hxx"
#include "DataTypes.hxx"

/**
 * @group Descriptors Pool
 */



namespace CLAM
{
	class AbstractAttribute
	{
	public:
		virtual ~AbstractAttribute() {}
		virtual void * Allocate(unsigned size) = 0;
		virtual void Deallocate(void * data) = 0;
		template <typename TypeToCheck>
		void CheckType() const
		{
			CLAM_ASSERT(typeid(TypeToCheck)==TypeInfo(),
				"Type Missmatch using a pool");
		}
	protected:
		virtual const std::type_info & TypeInfo() const = 0;
	};

	template <typename AttributeType>
	class Attribute : public AbstractAttribute
	{
	public:
		typedef TData DataType;
		virtual void * Allocate(unsigned size)
		{
			return new AttributeType[size];
		}
		virtual void Deallocate(void * data)
		{
			delete [] (AttributeType*)data;
		}
	protected:
		virtual const std::type_info & TypeInfo() const
		{
			return typeid(AttributeType);
		}
	};

	class AttributeScope
	{
	public:
		typedef std::map<std::string, unsigned> NamesMap;
		typedef std::vector<AbstractAttribute *> Attributes;
	private:
		NamesMap _nameMap;
		Attributes _attributes;
	public:
		~AttributeScope()
		{
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (; it!=end; it++)
				delete *it;
		}

		template <typename AttributeType>
		void Add(const std::string & name)
		{
			unsigned pos = _nameMap.size();
			bool inserted = 
				_nameMap.insert(std::make_pair(name,pos)).second;
			CLAM_ASSERT(inserted,"ScopeSpec::Add, Attribute already present");
			_attributes.push_back(new Attribute<AttributeType>);
		}

		unsigned GetIndex(const std::string & name) const
		{
			NamesMap::const_iterator it = _nameMap.find(name);
			CLAM_ASSERT(it!=_nameMap.end(),
				"Not such descriptor name on this descriptor scope");
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
		void CheckType(unsigned pos, AttributeType *) const
		{
			_attributes[pos]->CheckType<AttributeType>();
		}
	};

	class Pool
	{
	public:
		typedef std::vector<void*> Attributes;
	private:
		unsigned _size;
		Attributes _attributes;
		const AttributeScope & _spec;
	public:
		Pool(const AttributeScope & spec, unsigned size=0)
			: _size(0), _spec(spec)
		{
			_attributes.resize(_spec.GetNAttributes());
			Allocate(size);
		}
		~Pool()
		{
			Deallocate();
		}
	private:
		void Deallocate()
		{
			if (!_size) return;
			Attributes::iterator it = _attributes.begin();
			Attributes::iterator end = _attributes.end();
			for (unsigned i=0; it!=end; i++, it++)
				_spec.Deallocate(i, *it);
			_size=0;
		}
		void Allocate(unsigned newSize)
		{
			if (!newSize) return;
			_size = newSize;
			for (unsigned i = 0; i<_spec.GetNAttributes(); i++)
				_attributes[i]=_spec.Allocate(i,_size);
		}
	public:
		unsigned GetNAttributes()
		{
			return _spec.GetNAttributes();
		}
		unsigned GetSize()
		{
			return _size;
		}
		void SetSize(unsigned newSize)
		{
			Deallocate();
			Allocate(newSize);
		}

		template <typename AttributeType>
		const AttributeType * Get(const std::string & name) const
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			return (const AttributeType*) _attributes[attribPos];
		}

		template <typename AttributeType>
		AttributeType * Get(const std::string & name)
		{
			CLAM_ASSERT(_size,"Getting an attribute from a zero size pool");
			unsigned attribPos = _spec.GetIndex(name);
			_spec.CheckType(attribPos,(AttributeType*)0);
			return (AttributeType*) _attributes[attribPos];
		}
	};

	class ScopeRegistry
	{
	private:
		typedef std::map<std::string, unsigned> SpecMap;
		typedef std::vector<AttributeScope *> Specs;
	private:
		Specs _specs;
		SpecMap _specMap;
	public:
		ScopeRegistry()
		{
		}

		~ScopeRegistry()
		{
			Specs::iterator it = _specs.begin();
			Specs::iterator end = _specs.end();
			for (; it!=end; it++)
				delete *it;
		}

		template < typename AttributeSpec >
		void AddAttribute(const std::string &scope, const std::string & name)
		{
			typedef typename AttributeSpec::DataType DataType;
			AttributeScope & theSpec = SearchScopeOrAdd(scope);
			theSpec.template Add<DataType>(name);
		}

		AttributeScope & SearchScopeOrAdd(const std::string scopeName)
		{
			const unsigned nSpecs = _specs.size();
			std::pair<SpecMap::iterator,bool> result = 
				_specMap.insert(std::make_pair(scopeName,nSpecs));

			// Already inserted
			if (!result.second) return *_specs[result.first->second];

			AttributeScope * theSpec = new AttributeScope;
			_specs.push_back(theSpec);
			return *theSpec;
		}

		const AttributeScope & GetSpec(const std::string & name) const
		{
			SpecMap::const_iterator it = _specMap.find(name);
			CLAM_ASSERT(it!=_specMap.end(), "No scope registered with that name");
			return *_specs[it->second];
		}

		Pool * CreatePool(const std::string & scope)
		{
			return new Pool(GetSpec(scope));
			
		}
	};
}



