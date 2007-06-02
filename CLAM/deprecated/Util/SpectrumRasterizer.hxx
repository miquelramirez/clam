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

#ifndef _SPECTRUMRASTERIZER_
#define _SPECTRUMRASTERIZER_

#include "MapAlgo.hxx"
#include "LinearMapping.hxx"

#include "Raster.hxx"
#include "Assert.hxx"
#include "ErrGUI.hxx"
#include "Pixel.hxx"
#include "Array.hxx"
#include "CLAM_Math.hxx"
#include <valarray>
#include "CLAMGL.hxx"

using CLAM::DataArray;
using CLAM::TData;
using CLAM::DB;

namespace CLAMGUI
{
	/**
	 *  This singleton rasterizes i.e. maps scalar data contained in an array
	 *  into vectorial data in RGB space, beside generating a pixmap representing
	 *  that mapping
	 */
class SpectrumRasterizer
{
private:
	/** Singleton Instance */
	static SpectrumRasterizer* mInstance;
	
	std::valarray<RGB8>     mColorLUT;
	unsigned int            mLUTSize;
	TColorPath              mPath;
	MappingAlgo*            mMappingAlgorithm;

protected:

	void BuildSegment(int LUTstartIdx, int segmentSize, const Pixel& StartColor, const Pixel& EndColor);

	void DetermineColorsPerSegment(int* segmentSizes, int segmentNum);

	void BuildLUT();
 
	/**
	 *   Constructor. Note that it is private
	 *  @param size The desired number of colors available for doing the mapping
	 *  @param path The desired color ramp to be available for the mapping, specified as a list
	 *              of RGB-space vectors
	 */

	SpectrumRasterizer(int size, const TColorPath& path) 
		: mColorLUT( size ), mLUTSize(size), mPath(path)
	{
		BuildLUT();
		mMappingAlgorithm = new LinearMapping;
		mMappingAlgorithm->BuildColorMapping(mLUTSize, 8192, 3.0);
	}

public:

	/**
	 *  This method changes the mapping algorithm i.e. from a linear mapping
	 *  (the default) to something more interesting
	 *  @param a The Mapping Algorithm object
	 *  @param nMappings The Size of the Mapping Function (since it is tabulated)
	 */

	void SetUpMappingAlgorithm(MappingAlgo* a, int nMappings, float scaleFactor)
	{
		delete mMappingAlgorithm;
		mMappingAlgorithm = a;
		mMappingAlgorithm->BuildColorMapping(mLUTSize, nMappings, scaleFactor);
	}
			
	/**
	 *  This method returns an instance of SpectrumRasterizer if it has been already
	 *  instantiated. Otherwise it instantiates it. This accomplish that this object
	 *  is just instantiated <B>once</B>.
	 */
	static SpectrumRasterizer*  GetInstance(int	LUTSize,const TColorPath& path)
	{
		if (mInstance == NULL) mInstance = new SpectrumRasterizer(LUTSize, path);

		return mInstance;
	}

	/**
	 *  "Converts" the incoming Spectrum into a pixmap of the specified width
	 *  and color depth
	 *  @param s The Spectrum
	 *  @param width The Pixmap desired width
	 *  @param bpp The Color depth expressed as bytes-per-pixel ( 3 -> 24-bit color data)
	 */	

	inline void Rasterize( unsigned int row_idx, const DataArray& specIn, Raster& out )
	{
		unsigned int LUTidx = 0;
		RasterSlice row = out.row( row_idx );
		TData inv_est_max = TData( 1.0 / ( 2.0 * specIn.Size() ) );

		for ( unsigned int k = 0; k < unsigned(specIn.Size()); k++ )
			{
//		LUTidx = mMappingAlgorithm->GetIndex( TData( specIn[k] * inv_est_max ) );
	
		LUTidx = (unsigned int)(TData( specIn[k]  * inv_est_max ) * (mLUTSize - 1));
		CLAM_ASSERT( LUTidx < mLUTSize, "Mapping algorithm returned an index out of bonds of the LUT" );

		row[k] = mColorLUT[ LUTidx ];
			}
	}

	inline void LogDataRasterize( unsigned int row_idx, const DataArray& specIn, Raster& out )
	{
		unsigned int LUTidx = 0;
		RasterSlice row = out.row( row_idx );
		TData inv_est_max = TData( 1.0 / ( DB( specIn.Size() * TData(2.0), 20 ) ) );

		for ( unsigned int k = 0; k < unsigned(specIn.Size()); k++ )
		{
//			LUTidx = mMappingAlgorithm->GetIndex( TData( specIn[k]  * inv_est_max ) );
			LUTidx = (unsigned int)(TData( specIn[k]  * inv_est_max ) * (mLUTSize - 1));
			CLAM_ASSERT( LUTidx < mLUTSize, "Mapping algorithm returned an index out of bonds of the LUT" );

			row[k] = mColorLUT[ LUTidx ];
		}


	}

	/**
	 *  Destructor. Note that the pixmaps produced by the SpectrumRasterizer are owned
	 *  by the "clients" of this entity for memory management purposes. So do not expect
	 *  it to destroy them...
	 */
	virtual ~SpectrumRasterizer()
	{
	}
};

}

#endif // SpectrumRasterizer.hxx

