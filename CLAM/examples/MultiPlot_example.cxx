/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Spectrum.hxx" // imports CLAM::Spectrum declaration
#include "Array.hxx" // imports CLAM::Array declaration
#include "MultiPlot.hxx" // imports CLAMVM::MultiPlot declaration
#include "SystemPlots.hxx" // imports CLAMVM::SystemPlots declaration
#include "PlotExamplesHelper.hxx" // imports some trivial functions needed by the example
#include <iostream>
#include <exception>

int main( int argc, char** argv )
{
	try // Note the exception handling
	{
		// Continuing with what was shown on the SinglePlot_example.cxx here we will
		// see how we can make a plot to hold the graphical representation of several
		// functions at the same time - a feature very useful while debugging

		// Here we instantiate the spectrums we want to see...
		CLAM::Spectrum   signal;
		CLAM::Spectrum   lowPass;
		CLAM::Spectrum   bandPass;
		CLAM::Spectrum   highPass;
		CLAM::Spectrum   filteredSignal;

		// and here we put some data in to them
		generateSignalSpectrum( signal );
		generateLowPassFilter( lowPass );
		generateBandPassFilter( bandPass );
		generateHighPassFilter( highPass );

		// There's a quite significant difference between Plot objects that can
		// hold just one function and those who can put as many functions
		// as might want: they have different interfaces. While the former implement
		// CLAMVM::Plot interface the later implement CLAMVM::MultiPlot interface, so
		// the question of 'which are the differences between them?' arises. We will
		// see it when we found it

		// For this example we will do with just two MultiPlot objects
		CLAMVM::MultiPlot  plotA( "plot_a" );
		CLAMVM::MultiPlot  plotB( "plot_b" );

		// Let's set some common settings for all representations
		plotA.SetPosition( 100, 100 );
		plotA.SetSize( 640, 480 );
		plotB.SetPosition( 100, 100 );
		plotB.SetSize( 640, 480 );

		// Since we are going to display spectral domain data we will adjust the
		// Y-axis range to something significant
		plotA.SetYRange( -120, 20 ); // between [-120:20] dBs
		plotB.SetYRange( -120, 20 );

		// And let's apply the low pass filter on to our signal
		filter( signal, lowPass, filteredSignal );
		
		// We will want to check and interesting fact: how the spectrum shape is
		// affected by the filter by drawing altogether the filter transfer function
		// spectrum and the original signal spectrum, and which are the differences
		// between the original and filtered signal by drawing together the original
		// and the filtered signal spectrums

		// First we set the color for the filter transfer function spectrum
		plotA.SetColor( "Transfer function spectrum", CLAMVM::Color( 255, 0, 0) );
		// and the the data - we will take the BPF representation
		plotA.AddData( "Transfer function spectrum", lowPass.GetMagBPF() );
		// And then we set the color and the data for the original signal spectrum
		plotA.SetColor( "Original signal spectrum", CLAMVM::Color( 0, 200, 0 ) );
		plotA.AddData( "Original signal spectrum", signal.GetMagBuffer(), 0.0, signal.GetSpectralRange() );
		// You should note two things:
		// 1) The difference in the interface of Plot and MultiPlot objects appears
		//    whenever we want to set the drawing color and the data to draw: similarly
		//    as we do already with plot objects themselves, we assign them a key ( a name )
		//    so to identify wich is the function we want to modify
		// 2) Plot::SetData method is mirrored onto MultiPlot::AddData
		// 3) BPF originated and Array originated drawings can be mixed freely

		// And now we will setup the plotB object
		plotB.SetColor( "Original signal spectrum", CLAMVM::Color( 255, 0, 0 ) );
		plotB.SetColor( "Filtered signal spectrum", CLAMVM::Color( 0, 200, 0 ) );
		plotB.AddData( "Original signal spectrum", signal.GetMagBuffer(), 0.0, signal.GetSpectralRange() );
		plotB.AddData( "Filtered signal spectrum", filteredSignal.GetMagBuffer(),
			       0.0, filteredSignal.GetSpectralRange() );

		// and we set the labels
		plotA.SetLabel( "H(f) -red- vs X(f) -green-" );
		plotB.SetLabel( "X(f) -red- vs H(f)·X(f) -green-" );
		
		CLAMVM::SystemPlots::DisplayAll();
		
		// Before finishing with this example, let's note a final detail: order matters, since functions
		// drawn first lie 'below' later drawn functions. This can be a bit disturbing in the first plot
		// so we can ammend it:

		// first, we clear the plot
		plotA.RemoveAllData();
		// and then, we reset colors and data ( in inverse order )
		
		plotA.SetColor( "Original signal spectrum", CLAMVM::Color( 0, 200, 0 ) );
		plotA.AddData( "Original signal spectrum", signal.GetMagBuffer(), 0.0, signal.GetSpectralRange() );
		plotA.SetColor( "Transfer function spectrum", CLAMVM::Color( 255, 0, 0) );
		plotA.AddData( "Transfer function spectrum", lowPass.GetMagBPF() );

		// and we just plot the "plot_a" object
		CLAMVM::SystemPlots::Display( "plot_a");

	}
	catch ( CLAM::Err& e) // First, we check wether the raised exception was a 'CLAM exception'
	{
		e.Print();
		exit(-1);
	}
	catch ( std::exception& e ) // If not a CLAM exception, we check if it is a standard
								// library exception
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	return 0;

}
	
