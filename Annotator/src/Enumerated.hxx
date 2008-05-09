/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef Enumerated_hxx
#define Enumerated_hxx
#include <typeinfo>
#include <CLAM/DynamicType.hxx>
#include <CLAM/XMLAdapter.hxx>
#include <CLAM/TypeInfo.hxx>
#include <string>

namespace CLAM_Annotator
{
	/* We need this stupid class in order to differentiate from a regular string
	when overloading functions*/
	class Enumerated:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Enumerated,1);
		DYN_ATTRIBUTE(0, public, std::string, String);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
	public:
		void operator = (const std::string& value)
		{
			SetString(value);
		}
		bool operator == (const std::string& value)
		{
			return (value == GetString());
		}
		void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(GetString(), 0, false);
			storage.Store(adapter);
		}
		void LoadFrom(CLAM::Storage & storage)
		{
			CLAM::XMLAdapter<std::string> adapter(GetString(), 0, false);
			if (!storage.Load(adapter))
				RemoveString();
		}
	};
};
namespace CLAM
{
	CLAM_TYPEINFOGROUP(CLAM::BasicCTypeInfo, CLAM_Annotator::Enumerated);
}
#endif//Enumerated_hxx

