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

#include "PeaksRenderingManager.hxx"
#include "Assert.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMVM
{
	PeaksRenderingManager::PeaksRenderingManager()
		: mMustProcessData( false ), mNoPeaksToShow( false ), mSpecRange( 22050 ), mNumBins( 513 ),
		  mLeftMostPeak( 0 )
	{
		mPeakRenderer.SetLineColor( 0, 0, 255 );
	}

	PeaksRenderingManager::~PeaksRenderingManager()
	{
	}

	void PeaksRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetPixelBounds( const ScreenBoundBox& scrbbox )
	{
		mScreenBBox = scrbbox;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetSpectralRange( TData srange )
	{
		mSpecRange = srange;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetBinNumber( TSize binNumber )
	{
		mNumBins = binNumber;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::CacheData( const Array<Partial>& array )
	{
		if ( mDataCached.Size() < array.Size() )
			mDataCached.Resize( array.Size() );
		mDataCached.SetSize( array.Size() );

		mLeftMostPeak = 0;
		mRightMostPeak = mDataCached.Size() - 1;

		std::copy( array.GetPtr(), array.GetPtr()+array.Size(), mDataCached.GetPtr() );

		if ( mProcessedX.Size() < array.Size() )
			mProcessedX.Resize( array.Size() );
		if ( mProcessedY.Size() < array.Size() )
			mProcessedY.Resize( array.Size() );

		mProcessedX.SetSize( array.Size()  );
		mProcessedY.SetSize( array.Size() );

		ConvertPartialsToArrays();

		mMustProcessData = true;
	}


	void PeaksRenderingManager::ProcessData()
	{
				
		DetermineVisibleSamples();

		TSize processedLen = (mRightMostPeak - mLeftMostPeak);				


		mPeakRenderer.SetXArray( mProcessedX.GetPtr()+mLeftMostPeak, processedLen );
		mPeakRenderer.SetYArray( mProcessedY.GetPtr()+mLeftMostPeak, processedLen );
		mPeakRenderer.SetYMinimum( mDataBBox.mBottom );

		mMustProcessData = false;
				
	}

	void PeaksRenderingManager::ConvertPartialsToArrays()
	{

		for ( TIndex peakIndex = 0; peakIndex < mProcessedX.Size(); peakIndex++ )
		{
			mProcessedX[peakIndex]= mDataCached[peakIndex].mFreq * ( mNumBins / mSpecRange );
			mProcessedY[peakIndex]= mDataCached[peakIndex].mMag;
		}
				
	}

	void PeaksRenderingManager::DetermineVisibleSamples()
	{
		TData minFreq = (mDataBBox.mLeft / mNumBins)*mSpecRange;
		TData maxFreq = (mDataBBox.mRight / mNumBins)*mSpecRange;

		TIndex i = 0;

		if ( mDataCached.Size() == 0 || mDataCached[0].mFreq > maxFreq )
		{
			mNoPeaksToShow = true;
			return;
		}
		mNoPeaksToShow = false;

		for ( ; (i < mDataCached.Size() ) 
				&& ( mDataCached[i].mFreq < minFreq ); i++ );

		mLeftMostPeak = i;

		for ( ; ( i < mDataCached.Size() ) 
				&& ( mDataCached[i].mFreq < maxFreq ); i++ );

		mRightMostPeak = i;
				
	}
		
	void PeaksRenderingManager::RenderData()
	{
		if ( !IsEnabled() )
			return;

		if ( mMustProcessData )
			ProcessData();
				
		mPeakRenderer.ExecuteGLCommands();
	}


}
