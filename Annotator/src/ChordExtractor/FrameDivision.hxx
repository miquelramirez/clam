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


#ifndef FrameDivision_hxx
#define FrameDivision_hxx

#include "DynamicType.hxx"

namespace Simac
{
class FrameDivision : public CLAM::DynamicType
{
	DYNAMIC_TYPE(FrameDivision,2);
	DYN_ATTRIBUTE(0, public, CLAM::TData, FirstCenter);
	DYN_ATTRIBUTE(1, public, CLAM::TData, InterCenterGap);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
public:
	CLAM::TData GetCenter(unsigned element)
	{
		return GetFirstCenter() + element* GetInterCenterGap();
	}
};

}

#endif//FrameDivision_hxx

