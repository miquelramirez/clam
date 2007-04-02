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
#include "QtSinTracksPlot.hxx"
#include "DPSinTracks.hxx"

namespace CLAM
{
    namespace VM
    {

		void PlotSinTracks( const Segment& segment,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtSinTracksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(segment);
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}

		void PlotSinTracks( const Array< SpectralPeakArray >& peakMtx, 
							const TData& sr, 
							const TData& dur,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtSinTracksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(peakMtx,sr,dur);
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}

		void PlotSinTracks( const Segment& segment,
							std::vector<unsigned>& marks,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtSinTracksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(segment);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}

		void PlotSinTracks( const Array< SpectralPeakArray >& peakMtx, 
							const TData& sr, 
							const TData& dur,
							std::vector<unsigned>& marks,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtSinTracksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(peakMtx,sr,dur);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}
					
    }
}

// END

