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

#ifndef __PEAKSRENDERINGMANAGER__
#define __PEAKSRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLPointsArrays.hxx"
#include "DataTypes.hxx"
#include "Partial.hxx"
#include "Array.hxx"

namespace CLAMVM
{
	using CLAM::Array;
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;
	using CLAM::TIndex;

	class PeaksRenderingManager : public GLRenderingManager
	{
	public:
		PeaksRenderingManager();
		virtual ~PeaksRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox& );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void SetSpectralRange( TData srange );
		virtual void SetBinNumber( TSize binNum );
		virtual void CacheData( const Array<Partial>& array );
		virtual void RenderData();

	protected:
				
		virtual void ProcessData();
		virtual void ConvertPartialsToArrays();
		virtual void DetermineVisibleSamples();

	private:
				
		Array<Partial>   mDataCached;
		DataArray        mProcessedX;
		DataArray        mProcessedY;
		GLPointsArrays    mPeakRenderer;
		DataBoundBox     mDataBBox;
		ScreenBoundBox   mScreenBBox;
		bool             mMustProcessData;
		bool             mNoPeaksToShow;
		TData            mSpecRange;
		TSize            mNumBins;
		TIndex           mLeftMostPeak;
		TIndex           mRightMostPeak;
	};

}


#endif // PeaksRenderingManager.hxx
