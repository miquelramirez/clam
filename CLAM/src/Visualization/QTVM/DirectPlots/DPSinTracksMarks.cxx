#include "QtAppWrapper.hxx"
#include "QtSinTracksPlot.hxx"
#include "DPSinTracksMarks.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSinTracksMarks(	const Segment& segment,
									std::list<unsigned>& marks,
									const std::string& label,
									int x, int y, int w, int h	 )
		{
			QtAppWrapper::Init();
			
			QtSinTracksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(segment);
			plot.SetMarks(marks);
			plot.SetMarksColor(marks);
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}

		void PlotSinTracksMarks(	const Array< SpectralPeakArray >& peakMtx, 
									const TData& sr, 
									const TData& dur,
									std::list<unsigned>& marks,
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

