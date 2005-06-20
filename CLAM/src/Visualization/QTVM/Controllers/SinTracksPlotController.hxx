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

#ifndef __SINTRACKSPLOTCONTROLLER__
#define __SINTRACKSPLOTCONTROLLER__

#include "Segment.hxx"
#include "SinTrackHClipper.hxx"
#include "SinTrackVClipper.hxx"
#include "SinTracksRenderer.hxx"
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class SinTracksPlotController : public SelTimeRegionPlotController 
		{
			Q_OBJECT

		public:
			SinTracksPlotController();
			~SinTracksPlotController();

			void SetData(const Segment& segment);
			void SetData(const Array< SpectralPeakArray >& peakMtx, const TData& sr, const TData& dur);
			void SurfaceDimensions(int w,int h);
			void Draw();

			void SetMousePos(TData x,TData y);

			bool IsPlayable();
	
		protected:
			void SetHBounds(const TData& left,const TData& right);
			void SetVBounds(const TData& bottom,const TData& top);

		private:
			SineTrackList      mCachedTracks;
			SinTrackHorClipper mHorClipper;
			SinTrackVerClipper mVerClipper;
			SinTracksRenderer  mRenderer;
			SineTrackSpanEnds  mStarts;
			SineTrackSpanEnds  mEnds;
			bool               mMustProcessData;
			int	               mNumberOfFrames;

			void FullView();

			void ProcessData();
			void Colorize();

			float ClampToRange(TData value) const;
			void InitialRegionTime();
			
		};
    }
}

#endif

