#include "QtAppWrapper.hxx"
#include "QtSinTracksPlot.hxx"
#include "DPSinTracks.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSinTracks(	const Segment& segment,
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

		void PlotSinTracks(	const Array< SpectralPeakArray >& peakMtx, 
							const TData& sr, 
							const TData& dur,
							const std::string& label="",
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
					
	}
}

// END

