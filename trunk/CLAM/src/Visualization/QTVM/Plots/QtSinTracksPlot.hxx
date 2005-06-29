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

#ifndef __QTSINTRACKSPLOT__
#define __QTSINTRACKSPLOT__

#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "SingleDisplayPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		class SingleLabel;

		/**
		 * Allows viewing sinusoidal tracks.
		 *
		 * @ingroup QTVM
		 */

		class QtSinTracksPlot : public SingleDisplayPlot
		{
			Q_OBJECT

		public:
			QtSinTracksPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0);
			~QtSinTracksPlot();

			void SetData(const Segment& segment);
			void SetData(const Array< SpectralPeakArray >& peakMtx, 
						 const double& sr, const double& dur	);

		protected:
			void SetPlotController();
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();

		private slots:
			void updateLabels(double, double);

		private:
			SingleLabel* mTimeLabel;
			SingleLabel* mFreqLabel;

			void InitSinTracksPlot();
		};
    }
}

#endif



