#include "QtAppWrapper.hxx"
#include "QtFundFreqPlot.hxx"
#include "DPFundamentalMarks.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotFundamentalMarks(	const Segment& segment,
									std::list<unsigned>& marks,
									const std::string& label,
									int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtFundFreqPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(segment);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}
					
	}
}

// END

