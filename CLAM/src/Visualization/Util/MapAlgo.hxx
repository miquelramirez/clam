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

#ifndef _MAPPINGALGO_
#define _MAPPINGALGO_

#include "DataTypes.hxx"

using CLAM::TData;

namespace CLAMGUI
{
	/**
	 *	Abstract class defining an interface for class that encapsulate Color mapping algorithms
	 */
	class MappingAlgo
	{
		public:
			/**
			 *	Builds a Lookup Table for the color mapping function
			 *  @param LUTSize Size of the source Color Lookup Table
			 *  @param ColorMappingSize Size for the Lookup Table of the Mapping function. It defines its resolution
			 */
			virtual void BuildColorMapping(int LUTSize, int ColorMappingSize, TData scaleFactor) = 0;
			/**
			 *	For a given value, returns the index in the source Color Lookup Table
			 *  thus giving us the "mapped" color for that value
			 *  @param value The value 
			 *  @return The index in the source Lookup Table
			 */
			virtual int	 GetIndex(TData value) const = 0;
			/**
			 *	Destructor
			 */
			virtual ~MappingAlgo() {}
	};
}

#endif // MappingAlgo.hxx

