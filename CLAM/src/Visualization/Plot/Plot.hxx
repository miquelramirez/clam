#ifndef __CLAMVM_PLOT_HXX
#define __CLAMVM_PLOT_HXX

/**
 * @file Plot.hxx
 *
 * Plots provide a means of visual inspecting the content 
 * of ProcessingData by means of plotting them. Their main 
 * purpose is as a debugging aid and, in general, should 
 * never be used in production code.
 *
 * Their interface is as follows:
 * @verbatim
   plot(const ModelT &model, const char *label)
   @endverbatim
 * and overloads are provided for different ModelT 
 * types, such as CLAM::Audio, CLAM::Spectrum, etc.
 *
 * The Plots are implemented using the fltk GUI toolkit 
 * and are shown as modal windows. That is, the application 
 * flow is 'blocked' until the Plot's window is closed by 
 * the application user.
 */

#include "PlotAudio.hxx"
#include "PlotSpectrum.hxx"
#include "PlotSinTracks.hxx"
#include "PlotSpectralPeaks.hxx"

namespace CLAMVM
{
	void showPendingPlots();
}

#endif // __CLAMVM_PLOT_HXX
