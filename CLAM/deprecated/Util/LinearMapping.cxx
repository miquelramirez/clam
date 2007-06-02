/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "LinearMapping.hxx"
using namespace CLAMGUI;

void LinearMapping::BuildColorMapping(int LUTSize, int ColorMappingSize, TData scaleFactor) 
{
Init(ColorMappingSize);


for (int i = 0; i < mMapSize ; i++ )
{
mColorMappings[i] = i;
}
}

int LinearMapping::GetIndex(TData value) const
{
	if (value >1.0) return mColorMappings[mMapSize-1];
	if (value < 0.5 && value > 0.05) value+=0.5;
	return mColorMappings[static_cast<int>(value*static_cast<TData>(mMapSize) )];				
}

