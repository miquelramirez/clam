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

#include "DescriptionScheme.hxx"
#include "XMLAdapter.hxx"

namespace CLAM
{
	DescriptionScheme::~DescriptionScheme()
	{
			Scopes::iterator it = _scopes.begin();
			Scopes::iterator end = _scopes.end();
			for (; it!=end; it++)
				delete *it;
	}
	namespace Implementation
	{
		class AttributeInserter : public Component
		{
		public:
			AttributeInserter(DescriptionScheme & scheme) : _scheme(scheme) {}
			const char * GetClassName() const {return "AttributeInserter"; }
			void StoreOn(Storage & storage) const
			{
			}
			void LoadFrom(Storage & storage)
			{
				std::string _name;
				XMLAdapter<std::string> nameAdapter(_name, "name", false);
				storage.Load(nameAdapter);

				std::string _scope;
				XMLAdapter<std::string> scopeAdapter(_scope, "scope", false);
				storage.Load(scopeAdapter);

				std::string _type;
				XMLAdapter<std::string> typeAdapter(_type, "type", false);
				storage.Load(typeAdapter);

				if (_type == "Float")
					_scheme.AddAttribute<float>(_scope, _name);
				else if (_type == "String")
					_scheme.AddAttribute<std::string>(_scope, _name);
			}
		private:
			DescriptionScheme & _scheme;
		};
	}
	
	void DescriptionScheme::StoreOn(Storage & storage) const
	{
		for (unsigned int i = 0; i < GetNScopes(); i++)
		{
			const DescriptionScope & scope = GetScope(i);
			for (unsigned int j = 0; j < scope.GetNAttributes(); j++)
			{
				XMLComponentAdapter adapter(scope.GetAttribute(j), "Attribute", true);
				storage.Store(adapter);
			}
		}
	}

	void DescriptionScheme::LoadFrom(Storage & storage)
	{
		for (; true; )
		{
			Implementation::AttributeInserter inserter(*this);
			XMLComponentAdapter adapter(inserter, "Attribute", true);
			if (!storage.Load(adapter)) break;
		}
	}
}


