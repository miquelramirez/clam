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

#ifndef __QTPLOTS__
#define __QTPLOTS__

#include "QtAudioPlot.hxx"
#include "QtAudioStereoPlot.hxx"
#include "QtFundFreqPlot.hxx"
#include "QtSpectrumPlot.hxx"
#include "QtSinTracksPlot.hxx"
#include "QtMultiPlot.hxx"
#include "QtSpectrogram.hxx"
#include "BPFEditor.hxx"

/**
 *@defgroup QTVM QT Visualization Module
 *@ingroup GraphicalInterface
 *
 * The namespace CLAM::VM offers a set of classes to view the data contained CLAM's 
 * ProcessingData. Various tools are available which allow the user to visually check 
 * CLAM::Audio, CLAM::Spectrum, and other objects.
 *
 * This set of classes has been implemented using Trolltech's Qt toolkit. These widgets 
 * can be integrated into other Qt applications, or they can use plots independently through 
 * DirectPlots or QtPlotter.
 *
 * Following sections focuses on showing how to use this set of widgets to view data.  
 * So, only the 'productive' classes are shown. The other types of classes are not 
 * discussed even though they form part of the system as the user does 
 * not need to make use of them directly.
 *
 * Use of the plots is fairly simple. If you are looking for a quick view of the data 
 * for feedback during debugging, it is best to use DirectPlots or QtPlotter.
 *
 *@section Plots Plots
 *
 * The QtPlots are real widgets and can be integrated into a user's own Qt application.
 *
 * - CLAM::VM::QtAudioPlot
 * - CLAM::VM::QtAudioStereoPlot
 * - CLAM::VM::QtFundFreqPlot
 * - CLAM::VM::QtMultiPlot
 * - CLAM::VM::QtSinTracksPlot
 * - CLAM::VM::QtSpectrogram
 * - CLAM::VM::QtSpectrumPlot
 *
 *@section DirectPlots Direct plots
 * 
 * DirectPlots allows the viewing of one specific plot, and until the window 
 * containing the plot is closed, no other plots can be viewed.
 *
 * - CLAM::VM::PlotAudio
 * - CLAM::VM::PlotAudioStereo
 * - CLAM::VM::PlotFundamental
 * - CLAM::VM::PlotSinTracks
 * - CLAM::VM::PlotSpectrogram
 * - CLAM::VM::PlotSpectrum
 * - CLAM::VM::PlotSpectrumAndPeaks
 * - CLAM::VM::DPMultiPlot
 * - CLAM::VM::MultiBPFViewer
 *
 * Viewing audio data with a DirectPlot could be done as follows:
 @code 
 #include "DPAudio.hxx"
 
 int main()
 {
 	CLAM::Audio audio;
    	// get audio data
    	CLAM::VM::PlotAudio(audio);
    	return 0;
 }
 @endcode
 *
 * This call to the function PlotAudio(...) will show an unlabelled widget with default 
 * geometry. The functions for viewing the plots directly share a similar syntax, changing
 * only the type of data to be viewed. For example, if you wanted to view the fundamental 
 * frequency data, the previous main would be:
 @code
 #include "DPFundamental.hxx"

 int main()
 {
 	CLAM::Segment segment;
        // get F0
        CLAM::VM::PlotFundamental(segment);
        return 0;
 }
 @endcode
 *
 *@section Plotter QtPlotter
 *
 * If you wish to view a set of plots simultaneously, the static class QtPlotter is available.
 *
 * - CLAM::VM::QtPlotter
 *
 *For example, if you want to view an audio signal, its F0 
 * evolution and sinusoidal tracks, you could procede as follows:
 @code
 #include "QtPlotter"
 
 using CLAM::VM::QtPlotter;
 
 int main
 {
 	CLAM::Audio audio;
	CLAM::Segment segment;
	// get data
	// add plos to plotter depending on the data
	QtPlotter::Add("audio_plot",audio);
	QtPlotter::Add("fund_plot",segment,CLAM::VM::FundamentalPlot);
	QtPlotter::Add("sintracks_plot",segment,CLAM::VM::SinTracksPlot);
	// set labels
	QtPlotter::SetLabel("audio_plot","Audio");
	QtPlotter::SetLabel("fund_plot","Fundamental");
	QtPlotter::SetLabel("sintracks_plot","Sinusoidal tracks");
	// show all
	QtPlotter::Run();
 	return 0;
 }
 @endcode
 *
 * The first paramenter is a key to identify the plot. Note that when the data is a CLAM::Segment you must 
 * specify which plot you wish to see by means of a third parameter.
 *
 *@section BPFEditor BPFEditor
 *
 * BPFEditor is editable/auralizable plot with Audio and MIDI support. 
 * You can modify an existing BPF or create a new one.
 * Because the plot is auralizable, you are able to "compose" a chained melody 
 * from scratch too.
 * As well you can retrieve the CLAM::Melody and CLAM::MIDIMelody from the widget.
 *
 * - CLAM::VM::BPFEditor
 *
 *@section Interactions User-plot interactions
 *
 * The user can perform several interactions with plots using the mouse or keyboard-mouse 
 * combinations. 
 * Zoom in/out is a common feature available in all the plots.
 * Some plots are able to play some data. Playable plots are the
 * following:
 * - CLAM::VM::BPFEditor
 * - CLAM::VM::QtAudioPlot
 * - CLAM::VM::QtAudioStereoPlot
 * - CLAM::VM::QtFundFreqPlot
 *
 * Selecting regions can be made by the following way on plots provided with a time axis:
 * 
 * If the Shift key is pressed and the user clicks the left button of the mouse on a 
 * particular position, a region will be selected.
 *
 * Another feature that is available in all the plots is viewing and editing segmentation 
 * marks.
 *
 * If the mouse pointer is over an existing mark, the cursor shape will change showing 
 * that this mark can be moved to the left or right.
 *
 * If Insert key is pressed and the user clicks with the left button of the mouse on a 
 * place without an existing mark, a new mark will be added in this position.
 *
 * If Delete key is pressed and the mouse pointer is over an existing mark, the cursor  
 * will show a cross shape informing the user that the mark can be removed by clicking the 
 * mouse left button.
 * 
 * You can perform a segmentation from scratch too, adding, removing and moving 
 * segmentation marks.
 *
 * Insert, remove and modify points on CLAM::BPFEditor are done in the same way as described for 
 * segmentation marks.
 *
 *@section Screens Screenshots
 * 
 * \image html audio_plot.jpg
 *
 * \image html spec_peaks_plot.jpg
 *
 * \image html sintracks_plot.jpg
 *
 */

#endif

