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

#ifndef __SINTRACKSRENDERINGMANAGER__
#define __SINTRACKSRENDERINGMANAGER__

#include "GLRenderingManager.hxx"
#include "SineTracksDef.hxx"
#include "SinTrackHorClipper.hxx"
#include "SinTrackVerClipper.hxx"
#include "GLSineTracks.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Fl_Palette.hxx"

namespace CLAMVM
{
	using CLAM::Array;
	using CLAM::TSize;
	using CLAM::TIndex;
	using CLAM::TData;
		
	class SinTracksRM : public GLRenderingManager
	{
	public:
		SinTracksRM();
		virtual ~SinTracksRM();

		virtual void SetWorldBounds( const DataBoundBox& );
		virtual void SetPixelBounds( const ScreenBoundBox& );
		virtual void CacheData( SineTrackList& );
		virtual void RenderData();

	protected:
		virtual void ProcessData();
		virtual void DetermineVisibleSamples();
		virtual void Colorize();

		inline float ClampToRange( TData value );
				
	private:
		SineTrackList*       mCachedTracks;
		SinTrackHorClipper   mHorClipper;
		SinTrackVerClipper   mVerClipper;
		GLSineTracks         mRenderer;
		bool                 mMustProcessData;
		DataBoundBox         mDataBBox;
		ScreenBoundBox       mScreenBBox;
		SineTrackSpanEnds    mStarts;
		SineTrackSpanEnds    mEnds;
		Fl_Palette           mPalette;
		bool                 mColorsAssigned;
	};
}

#endif // SinTracksRenderingManager.hxx
