/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __CLAMVM_OLD_PLOTS_HXX
#define __CLAMVM_OLD_PLOTS_HXX

/**
 * @file OldPlots.hxx
 * @deprecated 
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
