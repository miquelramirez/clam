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

#include "LogMapping.hxx"
using namespace CLAMGUI;

void LogMapping::BuildColorMapping(int LUTSize, int ColorMappingSize, TData scaleFactor) 
{
	Init(ColorMappingSize);
	double max = pow(TData(10.0),scaleFactor);
	double dx = (max - 1.0) / mMapSize;
	double x0 = 1.0;


	TData x = x0;
	int i = 0;
	while (x <= max)
	{
		mColorMappings[i] = static_cast<int> (  (log10(x) / scaleFactor ) * static_cast<double>(LUTSize-1) );
		x= x + dx;

		CLAM_ASSERT( mColorMappings[i] < LUTSize, "Whoops! Something doesn't works well with the mapping function" );
		CLAM_ASSERT( i <= ColorMappingSize, "Hmmm! We're writing on memory not reserved by us!" );

		i++;
	}

}

int LogMapping::GetIndex(TData value) const
{
	return mColorMappings[static_cast<int>(value*static_cast<TData>(mMapSize) )];				
}

