#ifndef __VMQT_PLOT_SINTRACKS_H__
#define __VMQT_PLOT_SINTRACKS_H__

#include "vm_qapp_wrapper.hxx"
#include "vm_sintracks_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_sintracks(const Segment& s,
						   int x=100,
						   int y=100,
						   int w=600,
						   int h=250,
						   const std::string& title="SinTracks Plot",
						   bool bg_white=true)
		{
			CLAM::VM::QAppWrapper::init();
			CLAM::VM::SinTracksPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_data(s);
			(bg_white) ? plot.backgroundWhite() : plot.backgroundBlack();
			plot.show();
			return CLAM::VM::QAppWrapper::run();
		}
	}
}

#endif

