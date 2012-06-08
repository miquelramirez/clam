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

#include <CLAM/Segment.hxx>
#include <CLAM/SinTrackHClipper.hxx>
#include <CLAM/SinTrackVClipper.hxx>
#include <CLAM/SinTracksRenderer.hxx>
#include <CLAM/PlotController.hxx>

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
			void SetData(const Array< SpectralPeakArray >& peakMtx, double sr, double dur);
			void DisplayDimensions(int w, int h);
			void Draw();

			void SetMousePos(double x, double y);
			void SetSelPos(double value, bool render);

			void SaveScreen();

		signals:
			void sendTimeFreq(double, double);

		public slots:
			void setHBounds(double, double);
			void setVBounds(double, double);
			void setSelectedXPos(double);
	
		protected:
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);

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

