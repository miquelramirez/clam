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

#include "ArrayRenderer2D.hxx"
using namespace CLAMGUI;

ArrayRenderer2D::ArrayRenderer2D(const c3f &color)
	: Renderer2D(color)
{
}

void ArrayRenderer2D::InitArray( unsigned int nelems, TData first, TData last )
{
	mFirst = first;
	mLast  = last;
	SetDataSize(nelems);
}

void ArrayRenderer2D::SetXLimits(TData x_first, TData x_last)
{
	mFirst = x_first;
	mLast  = x_last;
	mDelta = (mLast - mFirst) / (mSize-1);
	ResetXValues();
}

void ArrayRenderer2D::SetDataSize(int size)
{
	mSize = size;
	mDelta = (mLast - mFirst) / (mSize-1);
	ResizeVertexArray(mSize);
	ResetXValues();
}

void ArrayRenderer2D::CacheData( const DataArray& array )
{

	unsigned nbins = array.Size();

	if ( nbins != mSize )
		ResizeVertexArray( nbins ); // Valarray resizing to accomodate the new CLAM Array

	DataTransform( array );
}
