#ifndef __VMQT_PLOT_BPF_H__
#define __VMQT_PLOT_BPF_H__

#include <string>
#include "vm_qapp_wrapper.hxx"
#include "vm_playable_bpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_bpf(BPF* bpf,
					 double xmin, double xmax,                   // x range
					 double ymin, double ymax,                   // y range
					 double xstep, double ystep,                 // x/y grid steps
					 int xzoom_steps=10, int yzoom_steps=6,      // x/y zoom steps 
					 ERulerScale xscale=CLAM::VM::eLinearScale,  // x scale
					 ERulerScale yscale=CLAM::VM::eLinearScale,  // y scale
					 int x=100, int y=100, int w=600, int h=250, // geometry
					 const std::string& title="BPF Plot")
		{
			CLAM::VM::QAppWrapper::init();
			CLAM::VM::PlayableBPFPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_xrange(xmin,xmax,xscale);
			plot.set_yrange(ymin,ymax,yscale);
			plot.set_zoom_steps(xzoom_steps,yzoom_steps);
			plot.set_grid_steps(xstep,ystep);
			plot.set_data(bpf);
			plot.readOnly(); 
			plot.show();
			return CLAM::VM::QAppWrapper::run();
		}
	}
}

#endif
