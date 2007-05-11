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
		~DescriptionDataPool();

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
			CLAM_ASSERT(_scopePools[scopeIndex],
				("Getting the size of scope '"+scopeName+"' but it is not populated").c_str());
			return _scopePools[scopeIndex]->GetSize();
		}

		void Insert(const std::string & scopeName, unsigned pos)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			CLAM_ASSERT(_scopePools[scopeIndex], "booooo");
			_scopePools[scopeIndex]->Insert(pos);
		}
		void Remove(const std::string & scopeName, unsigned pos)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			CLAM_ASSERT(_scopePools[scopeIndex], "booooo");
			_scopePools[scopeIndex]->Remove(pos);
		}

		/** @todo Should this method be deprecated?? */
		void InstantiateAttribute(const std::string & scopeName, const std::string & attributeName)
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			const DescriptionScope & scope = _scheme.GetScope(scopeIndex);
			scope.GetIndex(attributeName);
			CLAM_ASSERT(_scopePools[scopeIndex],
				("Instantianting '"+scopeName+":"+attributeName+"' but the scope is not yet populated").c_str());
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

			CLAM_ASSERT(_scopePools[scopeIndex],
				("Writting data on '"+scopeName+":"+attributeName+"' but the scope is not yet populated").c_str());

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

			CLAM_ASSERT(_scopePools[scopeIndex],
				("Reading data from '"+scopeName+":"+attributeName+"' but the scope is not yet populated").c_str());

			return _scopePools[scopeIndex]->template GetReadPool<AttributeType>(attributeName);
		}
		bool IsInstantiated(const std::string & scopeName, const std::string & attributeName) const
		{
			unsigned scopeIndex = _scheme.GetScopeIndex(scopeName);
			if (!_scopePools[scopeIndex]) return false;
			return _scopePools[scopeIndex]->IsInstantiated(attributeName);
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

