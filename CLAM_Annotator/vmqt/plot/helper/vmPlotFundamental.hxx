#ifndef __VMQT_PLOT_FUNDAMENTAL_H__
#define __VMQT_PLOT_FUNDAMENTAL_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmPlayableFundamentalPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotFundamental(const CLAM::Segment& s,
							int x=100,
							int y=100,
							int w=600,
							int h=250,
							const std::string& title="Fundamental Frequency Plot")
		{
			QAppWrapper::Init();
			PlayableFundamentalPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(s);
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif

