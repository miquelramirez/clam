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

#include "PointsRenderer2D.hxx"
using namespace CLAMGUI;

PointsRenderer2D::PointsRenderer2D(const c3f &color)
	: Renderer2D(color)
{
}

void PointsRenderer2D::InitArray( unsigned int nelems, TData first, TData last )
{
	mFirst = first;
	mLast  = last;
	SetDataSize(nelems);
}

void PointsRenderer2D::SetXLimits(TData x_first, TData x_last)
{
	mFirst = x_first;
	mLast  = x_last;
	ResetXValues();
}

void PointsRenderer2D::SetDataSize(int size)
{
	mSize = size;
	ResizeVertexArray(mSize);
	ResetXValues();
}

void PointsRenderer2D::CacheData(const DataArray& positions,
								 const DataArray& values,
								 int n_elems)
{
	if ( n_elems != mSize )
	{
		ResizeVertexArray(n_elems);
		mSize = n_elems;
	}
	DataTransform(positions,values);
}
