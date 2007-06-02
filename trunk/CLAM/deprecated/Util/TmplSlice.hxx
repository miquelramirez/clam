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

#ifndef __TMPLSLICE__
#define __TMPLSLICE__

#include <cstring>
#include "sizetdef.h"

namespace CLAMGUI{

/** 
 *  Curiosly parametrized version of Stroustrup's slice 
 */
template < unsigned Size, unsigned Stride >
class TmplSlice
{
	typedef   unsigned int  TSIZE_T;

	TSIZE_T mStartIdx;

	public:

	TmplSlice();

	TmplSlice( TSIZE_T start )
		: mStartIdx( start )
	{
	}

	TSIZE_T start() const
	{
		return mStartIdx;
	}

	TSIZE_T size() const
	{
		return Size;
	}

	TSIZE_T stride() const
	{
		return Stride;
	}

	TSIZE_T offset( TSIZE_T idx )
	{
		return mStartIdx + i*Stride;
	}

};

};

#endif // TmplSlice.hxx

