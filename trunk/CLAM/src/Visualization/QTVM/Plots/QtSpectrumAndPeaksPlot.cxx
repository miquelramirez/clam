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

#include "SpectrumAndPeaksPlotController.hxx"
#include "QtSpectrumAndPeaksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSpectrumAndPeaksPlot::QtSpectrumAndPeaksPlot(QWidget* parent, const char * name, WFlags f)
			: QtSpectrumPlot(parent,name,f)
		{
			SetPlotController();
			Connect();
		}
		
		QtSpectrumAndPeaksPlot::~QtSpectrumAndPeaksPlot()
		{
		}

		void QtSpectrumAndPeaksPlot::SetData(const Spectrum& spec,const SpectralPeakArray& peaks)		
		{			
		    ((SpectrumAndPeaksPlotController*)mController)->SetData(spec,peaks);
		}

		void QtSpectrumAndPeaksPlot::SetPeaksColor(Color cline,Color cpoint)
		{
			((SpectrumAndPeaksPlotController*)mController)->SetPeaksColor(cline,cpoint);
		}

		void QtSpectrumAndPeaksPlot::SetData(const Spectrum& spec)
		{
			// empty body for convenience
		}

		void QtSpectrumAndPeaksPlot::SetPlotController()
		{
			SetController(new SpectrumAndPeaksPlotController());
		}

		void QtSpectrumAndPeaksPlot::Connect()
		{
		    connect(((SpectrumAndPeaksPlotController*)mController),SIGNAL(mag(TData)),this,SLOT(updateMagLabel(TData)));
		    connect(((SpectrumAndPeaksPlotController*)mController),SIGNAL(freq(TData)),this,SLOT(updateFreqLabel(TData)));
		}

		void QtSpectrumAndPeaksPlot::DisplayBackgroundBlack()
		{
			QtSpectrumPlot::DisplayBackgroundBlack();
			SetPeaksColor(VMColor::Yellow(),VMColor::Red());
		}

		void QtSpectrumAndPeaksPlot::DisplayBackgroundWhite()
		{
			QtSpectrumPlot::DisplayBackgroundWhite();
			SetPeaksColor(VMColor::Cyan(),VMColor::Red());
		}
	}
}

// END



