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

#ifndef __AUDIORENDERINGMANAGER__
#define __AUDIORENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLMaxMinArrays.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{

	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;
	using CLAM::TTime;

	struct tAudioTimeInfo
	{
		TTime startTime;
		TTime endTime;
		TData sampleRate;
	};

	class GLStraightLineArray;
		

	class AudioRenderingManager : public GLRenderingManager
	{
	public:

		AudioRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox&  );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const DataArray& array, const tAudioTimeInfo& nfo );
		virtual void RenderData();
		const tAudioTimeInfo& GetTimeInfo(  );
		const DataArray& GetDataCached(  );

		void SetDetailThreshold( int nSamples )
		{
			mDetailThreshold = nSamples;
		}

		virtual ~AudioRenderingManager();

	protected:

		virtual void ProcessData();
		
		void ActivateStraightRenderer( );
		
		void ActivateDetailRenderer( );
		
		void BuildMaxMinArrays( TSize offset, TSize len );
		
		void DetermineVisibleSamples();

	private:
		
		DataArray                mDataCached;
		DataArray                mProcessedData;
		DataArray                mMaxs;
		DataArray                mMins;
		GLStraightLineArray*     mActiveRenderer;
		GLMaxMinArrays           mHugeArrayRenderer;
		int                      mDetailThreshold;
		DataBoundBox             mDataBBox;
		ScreenBoundBox           mScreenBBox;
		bool                     mMustProcessData;
		bool                     mHugeArrayCondition;
		tAudioTimeInfo           mCachedTimeInfo;
		TSize                    mOffset;
	};

}
#endif // AudioRenderingManager.hxx
