#include "QtAppWrapper.hxx"
#include "QtFundFreqPlot.hxx"
#include "DPFundamental.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotFundamental(	const Segment& segment,
								const std::string& label,
								int x, int y, int w, int h )
		{
			QtAppWrapper::Init();
			
			QtFundFreqPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(segment);
			plot.SwitchDisplayColors(true);
			plot.Show();
			
			QtAppWrapper::Run();
		}
					
	}
}

// END

