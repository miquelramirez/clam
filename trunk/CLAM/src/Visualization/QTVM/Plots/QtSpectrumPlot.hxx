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

#ifndef __QTSPECTRUMPLOT__
#define __QTSPECTRUMPLOT__

#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "SingleDisplayPlot.hxx"

namespace CLAM
{
    namespace VM
    {	

		class SingleLabel;

		/**
		 * Allows viewing a single spectrum or a single spectrum with his peaks.
		 *
		 * @ingroup QTVM
		 */

		class QtSpectrumPlot : public SingleDisplayPlot
		{

			Q_OBJECT

		public:
			QtSpectrumPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0);
			~QtSpectrumPlot();

			void SetData(const Spectrum& spec);
			void SetData(const Spectrum& spec, const SpectralPeakArray& peaks);

			void UpdateData(const Spectrum& spec);
			void UpdateData(const Spectrum& spec, const SpectralPeakArray& peaks);

			void SetForegroundColor(Color c);
			void SetPeaksColor(Color cline,Color cpoint);

		protected:				
			virtual void SetPlotController();
			virtual void Connect();
			virtual void DisplayBackgroundBlack();
			virtual void DisplayBackgroundWhite();

		private slots:
            void updateLabels(double,double);

		private:
			SingleLabel* mMagLabel;
			SingleLabel* mFreqLabel;
	    
			void InitSpectrumPlot();
		};
    }
}

#endif



