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

#ifndef __AUDIOPLOTCONTROLLER__
#define __AUDIOPLOTCONTROLLER__

#include "Audio.hxx"
#include "DataRenderer.hxx"
#include "SelTimeRegionPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class AudioPlotController : public SelTimeRegionPlotController
		{
			Q_OBJECT

		public:
			AudioPlotController();
			~AudioPlotController();

			void SetData(const Audio& audio);
			void SetDataColor(Color c);
			void SurfaceDimensions(int w,int h);
			void Draw();

			void SetSelPos(const TData& value);
			void SetMousePos(TData x,TData y);

			TData GetAmp(TData t) const;

			void SetSelectedXPos(double xpos);

		protected:
			void SetHBounds(const TData& left,const TData& right);
			void SetVBounds(const TData& bottom,const TData& top);

			void FullView();
			bool MustProcessData() const;
				
		private:
			DataRenderer mRenderer;
			Audio        mAudio;
			DataArray    mMaxs;
			DataArray    mMins;
			DataArray    mProcessedData;
			bool         mMustProcessData;
			bool         mHugeArrayCondition;
				
			void DrawAxis();

			void DetermineVisibleSamples();
			void ProcessData();
			void BuildMaxMinArrays(TSize offset,TSize len);

			void InitialRegionTime();

		};
    }
}

#endif

