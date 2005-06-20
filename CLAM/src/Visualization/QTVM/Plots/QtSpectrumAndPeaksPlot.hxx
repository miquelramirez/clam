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

#ifndef __QTSPECTRUMANDPEAKSPLOT__ 
#define __QTSPECTRUMANDPEAKSPLOT__

#include "SpectralPeakArray.hxx"
#include "QtSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
	      
		/** 
		 * Allows viewing a single spectrum with peaks.
		 *
		 * @ingroup QTVM
		 */

		class QtSpectrumAndPeaksPlot : public QtSpectrumPlot
		{
			Q_OBJECT

		public:
			QtSpectrumAndPeaksPlot(QWidget* parent=0, const char * name = 0, WFlags f = 0);
			~QtSpectrumAndPeaksPlot();

			void SetData(const Spectrum& spec,const SpectralPeakArray& peaks);
			void SetPeaksColor(Color cline,Color cpoint);

		protected:
			void SetData(const Spectrum& spec);
			void SetPlotController();
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();
			
		};
	}
}

#endif


