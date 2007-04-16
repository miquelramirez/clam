/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __BOUNDINGBOXES__
#define __BOUNDINGBOXES__

namespace CLAMVM
{

struct DataBoundBox
{
	float mLeft;
	float mRight;
	float mBottom;
	float mTop;
	float mNear;
	float mFar;				
};

struct ScreenBoundBox
{
	int pixel_left;
	int pixel_top;
	int pixel_width;
	int pixel_height;

	inline bool IsPointInside( int ex, int ey ) {

		return ( ex > pixel_left && ex < (pixel_left + pixel_width) ) 
			&& ( ey > pixel_top && ey < ( pixel_top + pixel_height ) );
	}
};

}

#endif // BoundingBoxes.hxx

