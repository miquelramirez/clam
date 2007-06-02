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

#ifndef _LOGMAPPING_
#define _LOGMAPPING_

#include "MapAlgo.hxx"
#include "Assert.hxx"
#include "CLAM_Math.hxx"

namespace CLAMGUI
{
	class LogMapping : public MappingAlgo
	{
	private:
		int*	mColorMappings;
		int	mMapSize;
		bool	mMapAllocated;
		int	mFirstIndex;

	public:
		LogMapping() 
			: mMapSize(0), mMapAllocated(false)
		{
		}

		void Init(int size)
		{
			mColorMappings = new int[size];
			mMapAllocated = true;
			mMapSize = size;
		}

		void BuildColorMapping( int LUTSize, int ColorMappingSize, TData scaleFactor );

		int GetIndex( TData value ) const;

		virtual ~LogMapping()
		{
			if (mMapAllocated)
			{
				delete [] mColorMappings;
			}
		}
	};

}

#endif

