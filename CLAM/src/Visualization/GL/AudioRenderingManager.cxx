#include "AudioRenderingManager.hxx"
#include "GLStraightLineArray.hxx"
#include "GLSampleDetailArray.hxx"
#include "GLMaxMinArrays.hxx"
#include "Assert.hxx"
#include <algorithm>
#include <iostream>


namespace CLAMVM
{
	AudioRenderingManager::AudioRenderingManager()
		: mActiveRenderer( NULL ), mDetailThreshold( 100 ), mMustProcessData( false ), mHugeArrayCondition( false ), mOffset(0)
	{
	}

	AudioRenderingManager::~AudioRenderingManager()
	{
		if ( mActiveRenderer!=NULL )
			delete mActiveRenderer;
	}

	void AudioRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void AudioRenderingManager::SetPixelBounds( const ScreenBoundBox& scrBBox )
	{
		mScreenBBox = scrBBox;

		mMustProcessData = true;
	}

	const tAudioTimeInfo& AudioRenderingManager::GetTimeInfo(  )
	{
		return mCachedTimeInfo;
	}

	const DataArray& AudioRenderingManager::GetDataCached(  )
	{
		return mDataCached;
	}

	void AudioRenderingManager::CacheData( const DataArray& data, const tAudioTimeInfo& timenfo )
	{
		mDataCached.Resize( data.Size() );
		mDataCached.SetSize( data.Size() );

		std::copy( data.GetPtr(), data.GetPtr()+data.Size(), mDataCached.GetPtr() );
		mCachedTimeInfo = timenfo;

		mMustProcessData = true;
	}

	void AudioRenderingManager::ActivateStraightRenderer(  )
	{
		if ( mActiveRenderer != NULL )
		{
			delete mActiveRenderer;
			mActiveRenderer = new GLStraightLineArray;
		}
		else
			mActiveRenderer = new GLStraightLineArray;
	}

	void AudioRenderingManager::ActivateDetailRenderer()
	{
		if ( mActiveRenderer != NULL )
		{
			delete mActiveRenderer;
			mActiveRenderer = new GLSampleDetailArray;
		}
		else
			mActiveRenderer = new GLSampleDetailArray;
				
	}

	void AudioRenderingManager::ProcessData()
	{
		DetermineVisibleSamples();
			
		if ( mHugeArrayCondition )
		{
			mHugeArrayRenderer.SetArrays( mMaxs.GetPtr(), mMins.GetPtr(), mMaxs.Size() );					
			return;
		}
			
		if ( mProcessedData.Size() <= mDetailThreshold )
		{
			ActivateDetailRenderer();
		}
		else
			ActivateStraightRenderer();
			
		mActiveRenderer->SetStartAndHopSize( mOffset, 1 );
		mActiveRenderer->SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
		mMustProcessData = false;

	}

	void AudioRenderingManager::RenderData()
	{
		if ( mDataCached.Size() == 0 )
			return;
		if ( mMustProcessData )
		{
			ProcessData();
		}
				
		if ( mHugeArrayCondition )
		{
			mHugeArrayRenderer.YChanged( mDataBBox.mTop, mDataBBox.mBottom );
			mHugeArrayRenderer.ExecuteGLCommands();
		}
		else
			mActiveRenderer->ExecuteGLCommands();
	}

	void AudioRenderingManager::DetermineVisibleSamples()
	{
		TSize offset, len;

		offset = mDataBBox.mLeft;
		len = (mDataBBox.mRight - mDataBBox.mLeft)+1;

		mOffset = offset;

		if ( len/5 >= mScreenBBox.pixel_width )
		{
			mHugeArrayCondition = true;
			BuildMaxMinArrays( offset, len );
			return;
		}
				
		mHugeArrayCondition = false;
						
		if ( mProcessedData.Size() <= len )
			mProcessedData.Resize( len+1 );

		mProcessedData.SetSize( len+1 );

		std::copy( mDataCached.GetPtr()+offset, mDataCached.GetPtr()+offset+len+1, mProcessedData.GetPtr() );
	}

	void AudioRenderingManager::BuildMaxMinArrays( TSize offset, TSize len )
	{
		TSize startSearch, endSearch, searchIntervalLen, searchRemIntervalLen;
		TSize nMaxs = mScreenBBox.pixel_width;

		if ( mMaxs.Size() < nMaxs )
			mMaxs.Resize( nMaxs );
		if ( mMins.Size() < nMaxs )						
			mMins.Resize( nMaxs );

		mMaxs.SetSize( nMaxs );

		mMins.SetSize( nMaxs );

		searchIntervalLen = len / (nMaxs);
		searchRemIntervalLen = len % (nMaxs);
		
		int firstPassIterations = ( searchRemIntervalLen ) ?  nMaxs-1: nMaxs;

		startSearch = offset;
		endSearch = startSearch + searchIntervalLen;				

		for  ( int i = 0; i < firstPassIterations; i++ )
		{
			mMaxs[i] = *std::max_element( mDataCached.GetPtr()+startSearch, mDataCached.GetPtr()+endSearch );
			mMins[i] = *std::min_element( mDataCached.GetPtr()+startSearch, mDataCached.GetPtr()+endSearch );
						
			startSearch = endSearch;
			endSearch += searchIntervalLen;	
		}
				
		if ( searchRemIntervalLen )
		{
			mMaxs[nMaxs-1] = *std::max_element( mDataCached.GetPtr()+startSearch, mDataCached.GetPtr()+startSearch+searchRemIntervalLen );
			mMins[nMaxs-1] = *std::min_element( mDataCached.GetPtr()+startSearch, mDataCached.GetPtr()+startSearch+searchRemIntervalLen );
		}
	}
}
