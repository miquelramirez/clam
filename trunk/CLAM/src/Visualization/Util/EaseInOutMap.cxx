/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "EaseInOutMap.hxx"
using namespace CLAMGUI;

void EaseInOutMap::Init(int size)
{
	mColorMappings = new int[size];
	mMapAllocated = true;
	mMapSize = size;
}

void EaseInOutMap::BuildColorMapping(int LUTSize, int ColorMappingSize, TData scaleFactor) 
{
	Init(ColorMappingSize);

	TData speed = 0.0f;
	TData value = 0.0f;
	TData target = static_cast<TData>(LUTSize-1)*scaleFactor;
	TData accel = (target-value)/(1.0f/4.0f);

	TData remainingTime = 1.0f;
	TData totalTime = 1.0f;
	TData deltaTime = totalTime / static_cast<TData>(mMapSize);
	int k = 0;

	while (remainingTime > 0)
	{
		remainingTime = remainingTime - deltaTime;

		if ( remainingTime < (totalTime/2) )
		{
			speed = speed - (accel*deltaTime);
		}
		else
			speed = speed + (accel*deltaTime);

		if (k==0)
		{
			mColorMappings[k] = static_cast<int>(speed*deltaTime);
		}
		else
		{
			mColorMappings[k] = mColorMappings[k-1] + static_cast<int>(speed*deltaTime);
			if (mColorMappings[k] >= LUTSize)
			{
				mColorMappings[k] = LUTSize-1;
			}
		}

		k++;
	}
}

int EaseInOutMap::GetIndex( TData value ) const
{
	return mColorMappings[static_cast<int>(value*static_cast<TData>(mMapSize) )];	
}
