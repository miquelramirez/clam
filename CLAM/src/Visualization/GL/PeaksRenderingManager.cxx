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

		if ( mDataCached[0].mFreq > maxFreq )
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
