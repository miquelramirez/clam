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

#ifndef _RASTER_
#define _RASTER_

#include "STLMatrix.hxx"
#include "Assert.hxx"

namespace CLAMGUI
{

	/**
	 *	This class is the all-around Pixmap
	 */

	struct RGB8
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;

		static unsigned int Size()
		{
			return 3;
		}

	};

	typedef Matrix< RGB8 > Raster;

	typedef Slice_iter< RGB8 > RasterSlice;
	typedef Slice_iter< RGB8 > CRasterSlice;

}

#endif // Raster.hxx

