#ifndef _Pool_hxx_
#define _Pool_hxx_

#include "ScopePool.hxx"
#include "DescriptionScheme.hxx"

namespace CLAM
{
	/**
	 * Contains the extracted data for a given description process.
	 * Its structure conforms the one defined by a DescriptionScheme
	 * specified when constructed.
	 *
	 * Data is stored for each attribute as a C array.
	 *
	 * The values can be dumped and restored to and from an XML file.
	 * @ingroup SemanticalAnalysis
	 */
	class DescriptionDataPool : public Component
	{
	public:
		/**
		 * Constructs a desciption data pool.
		 * By default all the scopes are size 0 and no attributes are instanciated.
		 * @param scheme the DescriptionScheme to be taken as specification.
		 */
		DescriptionDataPool(const DescriptionScheme & scheme)
			: _scheme(scheme), _scopePools(_scheme.GetNScopes(),(ScopePool*)0)
		{
			}
			~DescriptionDataPool()
		{
			ScopePools::iterator it = _scopePools.begin();
			ScopePools::iterator end = _scopePools.end();
			for (; it != end; it++)
				if (*it) delete *it;
		}

		/**
		 * Sets how many values will the attributes on the specified scope have.
		 * Sets the number of contexts (ie. number of notes) for the given Scope (Note), 
		 * so that every attribute registered for that scope will have a single value 
		 * for each context.
		 */
		void SetNumberOfContexts(const std::string & scopeName, unsigned size)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			_scopePools[scopeIndex] = new ScopePool(scope, size);
		}

		unsigned GetNumberOfContexts(const std::string & scopeName) const
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			CLAM_ASSERT(_scopePools[scopeIndex],"Getting the Scope size but it is not populated");
			return _scopePools[scopeIndex]->GetSize();
		}

		/** @todo Should this method be deprecated?? */
		void InstantiateAttribute(const std::string & scopeName, const std::string & attributeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			scope.GetIndex(attributeName);
			CLAM_ASSERT(_scopePools[scopeIndex], "Instantianting an attribute inside an unpopulated scope");
		}
		/**
		 * Returns a pointer to the C array of data
		 * with all the values for the specified attribute.
		 * The C array lenght will be the GetScopeSize(scope) long.
		 * This methods allocates the array value the first time is invoqued for an attribute.
		 * @pre Such attribute in such scope exists and the type is the one in the template.
		 */
		template <typename AttributeType>
		AttributeType * GetWritePool(const std::string & scopeName, const std::string & attributeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);

			CLAM_ASSERT(_scopePools[scopeIndex],"Accessing attribute data inside an unpopulated scope");

			return _scopePools[scopeIndex]->template GetWritePool<AttributeType>(attributeName);
		}
		/**
		 * Returns a pointer to the C array of data
		 * with all the values for the specified attribute.
		 * The C array lenght will be the GetScopeSize(scope) long.
		 * @pre Such attribute in such scope exists and the type is the one in the template.
		 * @pre GetWritePool, which allocates the data,
		 * 	has been previously called for this attribute and pool object.
		 */
		template <typename AttributeType>
		const AttributeType * GetReadPool(const std::string & scopeName, const std::string & attributeName) const
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);

			CLAM_ASSERT(_scopePools[scopeIndex],"Accessing attribute data inside an unpopulated scope");

			return _scopePools[scopeIndex]->template GetReadPool<AttributeType>(attributeName);
		}
	// Component Interface

		const char * GetClassName() const { return "DescriptionDataPool"; }
		void StoreOn(Storage & storage) const
		{
			for (unsigned i = 0; i<_scopePools.size(); i++)
			{
				XMLComponentAdapter adapter(*(_scopePools[i]), "ScopePool", true);
				storage.Store(adapter);
			}
		}
		void LoadFrom(Storage & storage)
		{
			for (unsigned i = 0; i<_scopePools.size(); i++)
			{
				const DescriptionScope & scope = _scheme.GetScope(i);
				_scopePools[i] = new ScopePool(scope,0);
				XMLComponentAdapter adapter(*(_scopePools[i]), "ScopePool", true);
				storage.Load(adapter);
			}
		}

	private:
		const DescriptionScheme & _scheme;
		typedef std::vector<ScopePool*> ScopePools;
		ScopePools _scopePools;
	};

}


#endif// _Pool_hxx_

