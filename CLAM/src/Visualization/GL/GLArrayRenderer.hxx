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

#ifndef __GLARRAYRENDERER__
#define __GLARRAYRENDERER__

#include "Array.hxx"
#include <valarray>
#include "CLAMGL.hxx"
#include "VertexFormats.hxx"
#include "ArrayRenderer.hxx"
using CLAM::DataArray;

namespace CLAMGUI
{

class GLArrayRenderer
	: public ArrayRenderer
{
	struct tCullingData
	{
		unsigned left, right, pixel_width;
	};
public:

	GLArrayRenderer( unsigned char red = 0, unsigned char gree = 255, unsigned char blu = 0 );

	virtual ~GLArrayRenderer()
	{
	}

	virtual void CacheData( const DataArray& array );
	virtual void Draw();
	virtual void PerformCulling( float left, float right, unsigned pixel_width );
	virtual void DefineViewport( const DataArray& array, Viewport& view_specs );

protected:

	void ResizeArray( unsigned int new_size);
	virtual void InitArray( unsigned int nelems ); // 1024 is assumed
	virtual void DataTransform( const DataArray& array);
	virtual void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer );
	virtual void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer );
	virtual float GetXConversionFactor()
	{
		return 1;
	}
	/** If data reduction is used, everytime a zoom or change of view is done, we
	 *	need to recompute the index boundaries 
	 */
	virtual void UpdateBounds();
	/** Finds maximums and minimums in array, it is the core of the data reduction
	 *	algorithm used when array size is more than @see mMinPointsToOptimize.
	 */
	virtual void FindMaxMin();

protected:

	std::valarray< c3f_v3f > mIntertwined;
	CLAM::Array< GLuint >  mElemIdxBuffer;
	unsigned       mLastIndex,mFirstIndex,mnMaxMin;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	bool          mDataChanged;
	bool          mCullingRequested;
	bool          mMustUpdateBounds;
	tCullingData  mCullingData;
	/** if array to visualize has more than mMinPointsToOptimize, a data reduction
	 *	algorithm is used to visualize the array*/
	unsigned mMinPointsToOptimize;
};


}

#endif // GLArrayRenderer.hxx









