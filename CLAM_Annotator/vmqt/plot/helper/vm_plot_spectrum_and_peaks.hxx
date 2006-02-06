#ifndef __VMQT_PLOT_SPECTRUM_AND_PEAKS_H__
#define __VMQT_PLOT_SPECTRUM_AND_PEAKS_H__

#include "vm_qapp_wrapper.hxx"
#include "vm_spectrum_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		int plot_spectrum_and_peaks(const Spectrum& s,
									const SpectralPeakArray p,
									int x=100,
									int y=100,
									int w=600,
									int h=250,
									const std::string& title="Spectrum and Peaks Plot",
									bool bg_white=true)
		{
			CLAM::VM::QAppWrapper::init();
			CLAM::VM::SpectrumPlot plot;
			plot.set_title(title.c_str());
			plot.set_geometry(x,y,w,h);
			plot.set_data(s,p);
			(bg_white) ? plot.backgroundWhite() : plot.backgroundBlack();
			plot.show();
			return CLAM::VM::QAppWrapper::run();
		}
	}
}

#endif

