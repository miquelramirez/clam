#ifndef __VMQT_PLOT_FUNDAMENTAL_H__
#define __VMQT_PLOT_FUNDAMENTAL_H__

#include <string>
#include "vm_qapp_wrapper.hxx"
#include "vm_playable_fundamental_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_fundamental(const CLAM::Segment& s,
							 int x=100,
							 int y=100,
							 int w=600,
							 int h=250,
							 const std::string& title="Fundamental Frequency Plot")
		{
			CLAM::VM::QAppWrapper::init();
			CLAM::VM::PlayableFundamentalPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_data(s);
			plot.show();
			return CLAM::VM::QAppWrapper::run();
		}
	}
}

#endif

