#include "SpectrumRenderingManager.hxx"
#include "Assert.hxx"
#include <algorithm>

namespace CLAMVM
{
	SpectrumRenderingManager::SpectrumRenderingManager()
		: mDrawAsLines( true ), mDrawAsDetail( false ), mDetailThreshold( 100 ), mMustProcessData( false ), mOffset(0), mLen(0)
	{
				
	}

	SpectrumRenderingManager::~SpectrumRenderingManager()
	{
	}
		
	void SpectrumRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void SpectrumRenderingManager::SetPixelBounds( const ScreenBoundBox& scrBBox )
	{
		mScreenBBox = scrBBox;

		mMustProcessData = true;
	}

	void SpectrumRenderingManager::CacheData( const DataArray& data )
	{
		if ( mDataCached.Size() < data.Size() )
			mDataCached.Resize( data.Size() );
		mDataCached.SetSize( data.Size() );
				
		std::copy( data.GetPtr(), data.GetPtr()+data.Size(), mDataCached.GetPtr() );
				
		mMustProcessData = true;				
	}

	void SpectrumRenderingManager::ActivateStraightRenderer()
	{
		mDrawAsLines = true;
		mDrawAsDetail = false;
	}
		
	void SpectrumRenderingManager::ActivateDetailRenderer()
	{
		mDrawAsDetail = true;
		mDrawAsLines = false;
	}

	void SpectrumRenderingManager::ProcessData()
	{
		DetermineVisibleSamples();
				
		if ( mProcessedData.Size() <= mDetailThreshold )
		{
			ActivateDetailRenderer();
			mDetailRenderer.SetStartAndHopSize( mOffset, 1 );
			mDetailRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
		}
		else
		{
			ActivateStraightRenderer();
			mLineRenderer.SetStartAndHopSize( mOffset, 1 );
			mLineRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
						
		}
				
		mMustProcessData = false;
	}

	void SpectrumRenderingManager::RenderData()
	{
		if ( mDataCached.Size() <= 0 ) // has data
				return;
		if ( mMustProcessData )
			ProcessData();
		if ( mDrawAsLines )
			mLineRenderer.ExecuteGLCommands();
		else
			mDetailRenderer.ExecuteGLCommands();
	}

	void SpectrumRenderingManager::DetermineVisibleSamples()
	{

		mOffset = mDataBBox.mLeft;
		mLen = (mDataBBox.mRight - mDataBBox.mLeft)+1;

		if ( mProcessedData.Size() <= mLen )
			mProcessedData.Resize( mLen+1 );

		mProcessedData.SetSize( mLen+1 );

		std::copy( mDataCached.GetPtr()+mOffset, mDataCached.GetPtr()+mOffset+mLen+1, mProcessedData.GetPtr() );
				
	}
}
