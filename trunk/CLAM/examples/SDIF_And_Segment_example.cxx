/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "SDIFIn.hxx"  // imports CLAM::SDIFIn declaration
#include "Segment.hxx" // imports CLAM::Segment ProcessingData object declaration
#include "Err.hxx"     // imports CLAM::Err exception class declaration
//#include "Plots.hxx"   // imports various CLAM Visualization Module plots declarations
//#include "SystemPlots.hxx" // imports CLAMVM::SystemPlots declaration
#include <QtGui/QFileDialog> // imports Qt file choose dialog
#include <QtGui/QApplication> // imports Qt application object
#include <iostream>

// CLAM supports two ways of representing an audio signal:
// i)  As a buffer containing the signal samples as well as 'metadata' such as
//     the sampling rate and the time ( in ms ) beginning and ending offsets,
//     represented either in memory as a CLAM::Audio object or encoded in a
//     well-formed XML document
// ii) As an 'SMS analysis segment'. An 'SMS analysis segment' is a collection of 'SMS analysis
//     frames': which are the tokens into the original signal is broken, each token
//     being analyzed separately. Besides these local descriptions of spectral 
//     characteristics ( such as the spectral peaks and the residual spectrum ) there
//     are also 'global' characteristics of a signal as a whole, such as the fundamental
//     frequency over time and the sinusoidal tracks observed in the signal. As their
//     name implies, those frames are the output of the SMS analysis algorithm. Why
//     was I saying this becomes an alternative representation of an audio signal? Just
//     because it conveys, approximately, the same information that the buffer of samples
//     plus nice cues on the dynamic and timbric properties of the analyzed signal. Note
//     also that the SMS Analysis results can be 'inversely mapped' back into audio samples
//     obtaining an 'equivalent' signal to the original one. In a nutshell, the SMS representation
//     of a signal is more convenient to us, because might allow us to represent in more
//     compact form the same information, and because it allows us to manipulate the signal
//     in several interesting ways. 
//
// Due to the richness and complexity of the information contained in a segment we support
// currently two formats for encoding this data: one, as the rest of CLAM objects, a well-formed
// XML document, and another, as a SDIF file. SDIF stands for 'Sound Description Interchange Format',
// and it is a joint effort by major research institutes on audio content extraction and manipulation
// for assuring interoperability, in the sense that people is able to 'share' info extracted
// from audio signals. In this example we will see how to:
// i)  restore a previously generated SMS Segment from a SDIF file
// ii) synthesize the segment to obtain the original signal

int main ( int argc, char** argv )
{
	QApplication a(argc,argv);
	try
	{
		// We will use Qt file dialog for letting making the process
		// of acquiring an SDIF file easier
		QString filename = QFileDialog::getSaveFileName(0,
			QObject::tr("Choose an sdif file"),
			QString::null,
			QObject::tr("(*.sdif)")
		);

		// If the user presses the 'Cancel' button that appears on the dialog we will
		// assume he wants to 'cancel' the execution
		if ( filename.isEmpty() )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}

		// Now we will instante the config object for the SDIFIn Processing
		CLAM::SDIFInConfig   sdifLoaderConfig;
		// and we set the filename to the one selected by the user
		sdifLoaderConfig.SetFileName( filename.toStdString().c_str() );

		// And now we instantiate the SDIFIn Processing object
		CLAM::SDIFIn sdifLoader;
		// and we configure it
		if ( !sdifLoader.Configure( sdifLoaderConfig ) )
		{
			// If the SDIF file loading file fails for some reason
			// an error message is printed out, so all we can do
			// is to abort execution
			std::cerr << "Aborted" << std::endl;
			exit(-1);
		}
		
		// And now we instantiate the CLAM::Segment object that will hold
		// the data stored in the SDIF file
		CLAM::Segment  loadedSegment;

		// we start the SDIF loader
		sdifLoader.Start();

		// we place the data stored in the file onto the newly created segment
	    sdifLoader.Do( loadedSegment );

		// and we stop the SDIF loader
		sdifLoader.Stop();

		// To illustrate what's the Segment for, we can take a look at the various
		// items of information it does contain. One of them is the Fundamental
		// frequency ( or pitch ) detected in the audio signal

		// You might already know CLAMVM various plots - if that is not the case
		// then please refer to the plots examples available, for a oresentation
		// of the general concept. However, in this example we will see ( and use )
		// the 'specific plots', plot objects devised for rendering concrete data
		// types that trascend from simple Array's or BPF's.
		// The first of them is the FundFreqPlot, which as its name implies plots
		// the fundamental frequency of a given segment

// TODO: Port this to qt4 vmqt
/*
		CLAMVM::FundFreqPlot         theFundFreqPlot( "f0_plot");
		// We set the plot window label
		theFundFreqPlot.SetLabel( "Fundamental frequency in a segment" );
		// We set the plot window size
		theFundFreqPlot.SetSize( 640, 480 );
		
		// we associate the plot with the segment that contains the data we want to
		// see
		theFundFreqPlot.SetData( loadedSegment );

		// Note that the frequency plot you will see is just an 'approximation' several
		// different factors may affect its exactitude
		CLAMVM::SystemPlots::Display( "f0_plot" );


		// Another interesting thing to see are the sinusoidal tracks found in
		// the signal
		CLAMVM::SinTracksPlot    theSinTracksPlot( "strc_plot");
		theSinTracksPlot.SetLabel( "Sinusoidal tracks in a segment" );
		theSinTracksPlot.SetSize( 640, 480 );

		// we associate the plot with the data
		theSinTracksPlot.SetData( loadedSegment );

		// An we see the plots
		CLAMVM::SystemPlots::Display( "strc_plot" );

		// Finally, the other important data that might be interesting to see is the
		// spectral peaks detected for a given frame, so we will get a 'peak array' 
		// at random - a peak array is the set of spectral peaks detected in a given
		// analysis frame
		CLAMVM::SpectrumAndPeaksPlot theSpecAndPeaksPlot("peaks_plot");
		theSpecAndPeaksPlot.SetLabel( "Detected peaks" );
		theSpecAndPeaksPlot.SetSize( 640, 480 );

		// We get the spectral peak array in the middle of the signal
		CLAM::SpectralPeakArray& peakArrayInTheMiddle =
			loadedSegment.GetFrame( loadedSegment.GetnFrames() / 2).GetSpectralPeakArray();


		theSpecAndPeaksPlot.SetData( peakArrayInTheMiddle, loadedSegment.GetSamplingRate()/2 );

		CLAMVM::SystemPlots::Display( "peaks_plot");
*/

	}
	catch( CLAM::Err& e)
	{
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}


	return 0;
}

