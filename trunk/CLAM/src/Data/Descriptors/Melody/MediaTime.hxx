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

#ifndef _MediaTime_
#define _MediaTime_
#include <typeinfo>

#include "DataTypes.hxx"
#include "ProcessingData.hxx"

#ifdef WIN32
	//XA: this should not be here but else does not compile
	#undef GetClassName
#endif

namespace CLAM {
	
//////////////////////////////////////////////////////
// Class MediaTime:
//
/** Media Time type definition                   *
 * This type propose a flexible way of defining  *
 * the temporal location of an audio segment     *
 * Types: {begin, end}, {begin, duration}        *
 * This type is thought to be used to define     *
 * some kind of probability when defining segment*
 * boundaries                                    *
 *                                               */

class MediaTime: public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MediaTime, 3, ProcessingData);
	DYN_ATTRIBUTE (0, public, TData, Begin);   
	DYN_ATTRIBUTE (1, public, TData, End);
	DYN_ATTRIBUTE (2, public, TData, Duration);
	
protected:	
	void DefaultInit();	
};

} // namespace CLAM

#endif


