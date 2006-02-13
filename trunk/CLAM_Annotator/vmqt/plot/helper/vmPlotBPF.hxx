#ifndef __VMQT_PLOT_BPF_H__
#define __VMQT_PLOT_BPF_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmPlayableBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotBPF(BPF* bpf,
					double xmin, double xmax,                   // x range
					double ymin, double ymax,                   // y range
					double xstep, double ystep,                 // x/y grid steps
					int xzoom_steps=10, int yzoom_steps=6,      // x/y zoom steps 
					ERulerScale xscale=CLAM::VM::eLinearScale,  // x scale
					ERulerScale yscale=CLAM::VM::eLinearScale,  // y scale
					int x=100, int y=100, int w=600, int h=250, // geometry
					const std::string& title="BPF Plot")
		{
			QAppWrapper::Init();
			PlayableBPFPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetXRange(xmin,xmax,xscale);
			plot.SetYRange(ymin,ymax,yscale);
			plot.SetZoomSteps(xzoom_steps,yzoom_steps);
			plot.SetGrid_steps(xstep,ystep);
			plot.SetData(bpf);
			plot.readOnly(); 
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif
