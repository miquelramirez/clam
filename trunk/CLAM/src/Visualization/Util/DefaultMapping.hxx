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

#ifndef _DEFAULTMAPPING_
#define _DEFAULTMAPPING_

#include "MapAlgo.hxx"

namespace CLAMGUI
{
	/**
	 *	This is a concrete mapping algorithm. More precisely it tries
	 *  to produce an exponential mapping i.e. while representing an
	 *  spectogram there are "more" color available for low magnitude
	 *  values and "less" for hight magnitued values, thus achieving
	 *  a higher level resolution while representing low magnitude values
	 */
	class DefaultMapping : public MappingAlgo
	{
	private:
		int*	mColorMappings;
		int		mMapSize;
		bool	mMapAllocated;
		int		mFirstIndex;
	public:

		DefaultMapping() : mMapSize(0), mMapAllocated(false)
		{
		}

		inline void Init(int size)
		{
			mColorMappings = new int[size];
			mMapAllocated = true;
			mMapSize = size;
		}

		virtual ~DefaultMapping()
		{
			if (mMapAllocated)
			{
				delete [] mColorMappings;
			}
		}

		void BuildColorMapping(int LUTSize, int ColorMappingSize, TData scaleFactor);

		int GetIndex(TData value) const;


	};
}

#endif // DefaultMapping.hxx

