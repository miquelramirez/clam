#ifndef __VMQT_PLOT_SPECTRUM_AND_PEAKS_H__
#define __VMQT_PLOT_SPECTRUM_AND_PEAKS_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotSpectrumAndPeaks(const Spectrum& s,
								 const SpectralPeakArray p,
								 int x=100,
								 int y=100,
								 int w=600,
								 int h=250,
								 const std::string& title="Spectrum and Peaks Plot",
								 bool bg_white=true)
		{
			QAppWrapper::Init();
			SpectrumPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(s,p);
			(bg_white) ? plot.backgroundWhite() : plot.backgroundBlack();
			plot.show();
			return QAppWrapper::Run();
		}
	}
}

#endif

