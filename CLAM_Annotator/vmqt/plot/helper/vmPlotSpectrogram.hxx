#ifndef __VMQT_PLOT_SPECTROGRAM_H__
#define __VMQT_PLOT_SPECTROGRAM_H__

#include <string>
#include "vmQAppWrapper.hxx"
#include "vmSpectrogramPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotSpectrogram(const Segment& s,
							int x=100,
							int y=100,
							int w=600,
							int h=250,
							const std::string& title="Spectrogram Plot",
							bool bg_white=true)
		{
			QAppWrapper::Init();
			SpectrogramPlot plot;
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

