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

#include "SequenceRenderingManager.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include "CLAM_Math.hxx"
#include <algorithm>

namespace CLAMVM
{
	SequenceRenderingManager::SequenceRenderingManager()
		: mDrawAsLines( true ), mDrawAsDetail( false ), mDetailThreshold( 100 ), mMustProcessData( false ), mOffset(0), mLen(0),
		  mLineColor( 0, 255, 0 ), mVisible_dx( 0.01 ), mStart_x( 0.0), mCulledAway( false )
		
	{
		mDataBounds.mLeft = 0.0;
		mDataBounds.mRight = 1.0;
		mDataBounds.mBottom = -1.0;
		mDataBounds.mTop = 1.0;
	}

	SequenceRenderingManager::~SequenceRenderingManager()
	{
	}
		
	void SequenceRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void SequenceRenderingManager::SetPixelBounds( const ScreenBoundBox& scrBBox )
	{
		mScreenBBox = scrBBox;

		mMustProcessData = true;
	}

	void SequenceRenderingManager::CacheData( const DataArray& data )
	{
		if ( mDataCached.Size() < data.Size() )
			mDataCached.Resize( data.Size() );
		mDataCached.SetSize( data.Size() );
				
		std::copy( data.GetPtr(), data.GetPtr()+data.Size(), mDataCached.GetPtr() );
		
		mOffset = 0;
		mLen = data.Size();
		mMustProcessData = true;				
	}

	void SequenceRenderingManager::ActivateStraightRenderer()
	{
		mDrawAsLines = true;
		mDrawAsDetail = false;
	}
		
	void SequenceRenderingManager::ActivateDetailRenderer()
	{
		mDrawAsDetail = true;
		mDrawAsLines = false;
	}

	void SequenceRenderingManager::ProcessData()
	{
		DetermineVisibleSamples();
				
		if ( mProcessedData.Size() <= mDetailThreshold )
		{
			ActivateDetailRenderer();
			mDetailRenderer.SetStartAndHopSize( mStart_x, mVisible_dx );
			mDetailRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
		}
		else
		{
			ActivateStraightRenderer();
			mLineRenderer.SetStartAndHopSize( mStart_x, mVisible_dx );
			mLineRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
		}
				
		mMustProcessData = false;
	}

	void SequenceRenderingManager::RenderData()
	{
		if ( mDataCached.Size() <= 0 ) // has data
				return;
		if ( mMustProcessData )
			ProcessData();

		if ( mCulledAway )
			return;

		if ( mDrawAsLines )
		{
			mLineRenderer.SetLineColor( mLineColor.r, mLineColor.g, mLineColor.b );
			mLineRenderer.ExecuteGLCommands();
		}
		else
		{
			mDetailRenderer.SetLineColor( mLineColor.r, mLineColor.g, mLineColor.b );
			mDetailRenderer.ExecuteGLCommands();
		}
	}

	void SequenceRenderingManager::SetDataYRange( double ymin, double ymax )
	{
		mDataBounds.mBottom = ymin;
		mDataBounds.mTop = ymax;
	}

	void SequenceRenderingManager::SetDataXRange( double xmin, double xmax )
	{
		mDataBounds.mLeft = xmin;
		mDataBounds.mRight = xmax;
	}

	void SequenceRenderingManager::DetermineVisibleSamples()
	{
		// Trivial rendering rejection

		mCulledAway = false;
		
		if ( mDataBounds.mRight <= mDataBBox.mLeft 
		     || mDataBounds.mLeft >= mDataBBox.mRight)
		{
			mCulledAway = true;
			return;
		}
		
		// Data culling 
		TData dataSpan = fabs( mDataBounds.mRight - mDataBounds.mLeft );
		TData visibleSpan = fabs(mDataBBox.mRight - mDataBBox.mLeft);
		TData shownSpan = 0.0;

		if ( mDataBounds.mLeft >= mDataBBox.mLeft 
		     && mDataBounds.mRight <= mDataBBox.mRight ) // data is included in visible span
		{
			mOffset = 0;
			mStart_x = mDataBounds.mLeft;
			mLen = mDataCached.Size();
			shownSpan = dataSpan;
		}
		else if ( mDataBounds.mLeft <= mDataBBox.mLeft
			  && mDataBounds.mRight >= mDataBBox.mRight ) // inverse inclusion
		{
			mOffset = (TSize)(( fabs( mDataBounds.mLeft - mDataBBox.mLeft )/dataSpan ) * mDataCached.Size());
			mStart_x = mDataBBox.mLeft;
			mLen = (TSize)(( 1.0 - ( fabs( mDataBounds.mRight - mDataBBox.mRight ) / dataSpan ) ) * mDataCached.Size());
			mLen -= mOffset;
			shownSpan = visibleSpan;
		}
		else if ( mDataBounds.mLeft >= mDataBBox.mLeft 
			  && mDataBounds.mRight >= mDataBBox.mRight ) // left partial inclusion
		{
			mOffset = 0;
			mStart_x = mDataBounds.mLeft;
			mLen = (TSize)(( fabs( mDataBBox.mRight - mDataBounds.mLeft ) / dataSpan ) * mDataCached.Size());
			shownSpan = fabs( mDataBounds.mLeft - mDataBBox.mRight );
		}
		else if ( mDataBounds.mLeft <= mDataBBox.mLeft 
			  && mDataBounds.mRight <= mDataBBox.mRight ) // right partial inclusion
		{
			mOffset = (TSize)(( fabs( mDataBounds.mLeft - mDataBBox.mLeft )/dataSpan ) * mDataCached.Size());
			mStart_x = mDataBBox.mLeft;
			mLen = mDataCached.Size() - mOffset;
			shownSpan = fabs( mDataBounds.mRight - mDataBBox.mLeft );
		}
		else
		{
			CLAM_ASSERT( false, "Should never happen!" );
		}

		if ( mProcessedData.Size() < mLen )
			mProcessedData.Resize( mLen );

		mProcessedData.SetSize( mLen );

		std::copy( mDataCached.GetPtr()+mOffset, mDataCached.GetPtr()+mOffset+mLen, mProcessedData.GetPtr() );
		
		mVisible_dx = shownSpan / TData(mProcessedData.Size()-1);
				
	}
}
