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

#ifndef __POINTSRENDERER2D__
#define __POINTSRENDERER2D__

#include "Array.hxx"
#include "CLAMGL.hxx"
#include "VertexFormats.hxx"
#include "Renderer2D.hxx"
using CLAM::DataArray;
using CLAM::TData;

namespace CLAMGUI
{

class PointsRenderer2D : public Renderer2D
{

public:

	PointsRenderer2D(const c3f &color);

	virtual ~PointsRenderer2D() {}

	void SetXLimits(TData x_first, TData x_last);

	void SetDataSize(int max_size);

	void CacheData(const DataArray& positions,
				   const DataArray& values,
				   int n_elems);

	virtual void Draw(TData x_start, TData x_width) = 0;

protected:

	virtual void ResetXValues() = 0;

	virtual void ResizeVertexArray(unsigned n_data_elems) = 0;

	virtual void DataTransform(const DataArray& positions,
							   const DataArray& values) = 0;

	void InitArray( unsigned int nelems, TData first, TData last  );

	CLAM::Array<v2f> mVertex;

	unsigned mSize;
	TData    mFirst;
	TData    mLast;
};


}

#endif // PointsRenderer2D.hxx
