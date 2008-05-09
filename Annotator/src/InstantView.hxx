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

#ifndef InstantView_hxx
#define InstantView_hxx
#include <typeinfo>
#include <CLAM/DynamicType.hxx>

#include <list>
#include <CLAM/Text.hxx>
#include <CLAM/Enum.hxx>

#include <CLAM/Assert.hxx>

namespace CLAM_Annotator{
	
	class InstantView :public CLAM::DynamicType
	{
		DYNAMIC_TYPE(InstantView,3);
		DYN_ATTRIBUTE(0, public, std::string, Type); ///< The type name. Should be at the Type factory.
		DYN_ATTRIBUTE(1, public, std::string, AttributeScope); ///< The scope name at which the attribute sticks.
		DYN_ATTRIBUTE(2, public, std::string, AttributeName); ///< The attribute name, unique within the scope.
		
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
	};
};

#endif

