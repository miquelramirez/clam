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

#ifndef __SPECTRUMRENDERINGMANAGER__
#define __SPECTRUMRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "GLStraightLineArray.hxx"
#include "GLSampleDetailArray.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TSize;

	class SpectrumRenderingManager : public GLRenderingManager
	{
	public:
		SpectrumRenderingManager();
		virtual ~SpectrumRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox&  );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( const DataArray& array );
		virtual void RenderData();
				
		void SetDetailThreshold( int nSamples )
		{
			mDetailThreshold = nSamples;
		}
				
	protected:
		
		virtual void ProcessData();
				
		void ActivateStraightRenderer( );
				
		void ActivateDetailRenderer( );

		void DetermineVisibleSamples();

	private:			
				
		DataArray                mDataCached;
		DataArray                mProcessedData;
		GLStraightLineArray      mLineRenderer;
		bool                     mDrawAsLines;
		GLSampleDetailArray      mDetailRenderer;
		bool                     mDrawAsDetail;
		int                      mDetailThreshold;
		bool                     mMustProcessData;
		DataBoundBox             mDataBBox;
		ScreenBoundBox           mScreenBBox;
		TSize                    mOffset;
		TSize                    mLen;
	};
		
}

#endif // SpectrumRenderingManager.hxx
