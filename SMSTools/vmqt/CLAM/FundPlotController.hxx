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

#ifndef __FUNDPLOTCONTROLLER__
#define __FUNDPLOTCONTROLLER__

#include "Segment.hxx"
#include "DataRenderer.hxx"
#include "PlayablePlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		class FundPlotController : public PlayablePlotController
		{
			Q_OBJECT

		public:
			FundPlotController();
			~FundPlotController();

			void SetData(const Segment& segment);
			void SetDataColor(Color c);
			void DisplayDimensions(int w, int h);
			void Draw();

			void SetSelPos(double value, bool render);
			void SetMousePos(double x, double y);

		public slots:
			void setHBounds(double, double);
			void setVBounds(double, double);
			void setSelectedXPos(double);

		protected:
			void SetHBounds(double left,double right);
			void SetVBounds(double bottom, double top);

			void FullView();

		private:
			const Segment* mSegment;
			DataRenderer   mRenderer;
			DataArray      mCacheData;
			DataArray      mProcessedData;
			bool           mMustProcessData;
			bool           mHasData;
			
			void CacheData();
			void ProcessData();
			void SetRenderingStep();

			void InitialRegionTime();
		};
    }
}

#endif

