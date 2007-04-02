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

#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
#include "DPSpectrum.hxx"

namespace CLAM
{
    namespace VM
    {

		void PlotSpectrum( const Spectrum& spec,
						   const std::string& label,
						   int x, int y, int w, int h)
		{
			QtAppWrapper::Init();

			QtSpectrumPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetBackgroundColor(VMColor::White());
			plot.SetForegroundColor(VMColor::Blue());
			plot.SetDialColor(VMColor::Black());
			plot.SwitchDisplayColors(true);
			plot.SetData(spec);
			plot.Show();

			QtAppWrapper::Run();
		}

		void PlotSpectrum( const Spectrum& spec,
						   std::vector<unsigned>& marks,
						   const std::string& label,
						   int x, int y, int w, int h)
		{
			QtAppWrapper::Init();

			QtSpectrumPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetBackgroundColor(VMColor::White());
			plot.SetForegroundColor(VMColor::Blue());
			plot.SetDialColor(VMColor::Black());
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.SetData(spec);
			plot.Show();

			QtAppWrapper::Run();
		}
    }
}

// END

