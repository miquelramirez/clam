#ifndef __VMQT_PLOT_SINTRACKS_H__
#define __VMQT_PLOT_SINTRACKS_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmSinTracksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotSinTracks(const Segment& s,
						  int x=100,
						  int y=100,
						  int w=600,
						  int h=250,
						  const std::string& title="SinTracks Plot",
						  bool bg_white=true)
		{
			QAppWrapper::Init();
			SinTracksPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(s);
			(bg_white) ? plot.backgroundWhite() : plot.backgroundBlack();
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif

