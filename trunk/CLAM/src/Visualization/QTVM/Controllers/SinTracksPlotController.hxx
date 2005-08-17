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
#include "PlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class SinTracksPlotController : public PlotController 
		{
			Q_OBJECT

		public:
			SinTracksPlotController();
			~SinTracksPlotController();

			void SetData(const Segment& segment);
			void SetData(const Array< SpectralPeakArray >& peakMtx, const double& sr, const double& dur);
			void DisplayDimensions(const int& w, const int& h);
			void Draw();

			void SetMousePos(const double& x, const double& y);
			void SetSelPos(const double& value, bool render);

		signals:
			void sendTimeFreq(double, double);

		public slots:
			void setHBounds(double, double);
			void setVBounds(double, double);
			void setSelectedXPos(double);
	
		protected:
			void SetHBounds(const double& left, const double& right);
			void SetVBounds(const double& bottom,const double& top);

			void FullView();

		private:
			SineTrackList      mCachedTracks;
			SinTrackHorClipper mHorClipper;
			SinTrackVerClipper mVerClipper;
			SinTracksRenderer  mRenderer;
			SineTrackSpanEnds  mStarts;
			SineTrackSpanEnds  mEnds;
			double             mSampleRate;
			double             mDuration;
			bool               mHasData;
			bool               mMustProcessData;
			int	               mNumberOfFrames;

			void ProcessData();
			void Colorize();

			float ClampToRange(float value) const;
		   
		};
    }
}

#endif

