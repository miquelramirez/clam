/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_SEGMENTATION_PLOT_H__
#define __VMQT_SEGMENTATION_PLOT_H__

#include "vmWPlot.hxx"
#include "vmDataTypes.hxx"

class QGridLayout;

namespace CLAM
{
	class Segmentation;

	namespace VM
	{
		class Ruler;
		class Plot2D;
		class ScrollGroup;
		class SegmentEditor;
		class Locator;

		class SegmentationPlot : public WPlot
		{
			Q_OBJECT
		public:
			SegmentationPlot(QWidget* parent=0);
			virtual ~SegmentationPlot();
			
			virtual void SetXRange(double xmin, double xmax, ERulerScale scale=CLAM::VM::eLinearScale);
			virtual void SetYRange(double ymin, double ymax, ERulerScale scale=CLAM::VM::eLinearScale);			
			virtual void SetZoomSteps(int hsteps, int vsteps);
			
			virtual void setVisibleXRange(double min, double max);

			void SetSegmentation(Segmentation* s);
			
		signals:
			void selectedRegion(double, double);
			void segmentOnsetChanged(unsigned, double);
			void segmentOffsetChanged(unsigned, double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged();
			void visibleXRangeChanged(double,double);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

			void updateLocator(double);
			void updateLocator(double, bool);
			void setCurrentSegmentFollowsPlay(bool active);
			
		private slots:
			void setMaxVScroll(int);
			void setMaxHScroll(int);

		protected:
			QGridLayout* mLayout;    
			Plot2D*      mPlot;
			Ruler*       mXRuler;
			Ruler*       mYRuler;
			Locator*       mLocator;
			SegmentEditor* mSegmentation;
			bool mCurrentSegmentFollowsPlay;

		private:
			ScrollGroup* mHScroll;
			ScrollGroup* mVScroll;

			void InitSegmentationPlot();
			void AdjustYRulerWidth(double min, double max);
		};
	}
}

#endif

