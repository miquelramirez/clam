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
#include "Plot.hxx" // imports CLAMVM::Plot declaration
#include "SystemPlots.hxx" // imports CLAMVM::SystemPlots declaration
#include "PlotExamplesHelper.hxx" // imports some trivial functions needed by the example
#include <iostream>
#include <exception>

int main( int argc, char** argv )
{
	try // Note the exception handling
	{
		// In this example we will just use the code of the Frequency Domain Filter
		// Generator example, for acquiring some data interesting to see on the screen

		// Here we instantiate the spectrums we want to see...
		CLAM::Spectrum   lowPass;
		CLAM::Spectrum   bandPass;
		CLAM::Spectrum   highPass;

		// and here we put some data in to them
		generateLowPassFilter( lowPass );
		generateBandPassFilter( bandPass );
		generateHighPassFilter( highPass );

		// While developing DSP systems it's usual to have the need to 'see'
		// some data on the screen, just to check things are working the way
		// they are meant to. In CLAM, this can be achieved easily using the 
		// 'Visualization Module Plots'. The concept is very similar to that
		// of the MATLAB plot() function or GNUplot commands. But here, instead 
		// of functions or commands we will have to delve with objects, and
		// method invocation.

		// Our intent is to render on the screen the low pass filter frequency 
		// response. The first thing we have to do is to instantiate a CLAMVM::Plot
		// object. These kind of objects are meant for rendering on the screen 
		// CLAM::Array's and CLAM::BPFs, which are the containers offered by CLAM to 
		// perform our computations over signals.

		// First we instante a CLAMVM::Plot object. It is recommended to give them
		// a 'name': a global unique identifier attached to it. In this case we have
		// decided to name our plot as 'plot_1'.
		CLAMVM::Plot  lowPassPlot("plot_1");

		// We can give the plot several commands, that are useful to customize
		// it's appeareance. First of all we can set the label of the window that
		// will contain the actual data drawing
		lowPassPlot.SetLabel( "Low-pass filter frequency response");

		// We can also define the format wildcard for the tooltip text. This is useful
		// to chop out unwanted decimals or to give some 'chrome' to the data display
		// by adding info about the units of the two axis
		lowPassPlot.SetTooltipFormat( "f=%6.f Hz, H(f)=%3.2f dB" );

		// Other possibility is to set where the top left corner of the plot window
		// will lie
		lowPassPlot.SetPosition( 50, 50 );

		// We can also set the size ( in screen pixels ) for the plot window
		lowPassPlot.SetSize( 512, 512 );

		// It's also possible to set the color of the drawing. Color is specified as
		// a (r,g,b) triplet, where r is the red component intensity, g the green 
		// intensity and b the blue intensity. RGB values must be 8-bit wide i.e. 
		// between 0 and 255. Here we set the line color to be sort of 'light maroon'
		lowPassPlot.SetColor( CLAMVM::Color( 127, 150, 0 ) );

		// We may also want to define the y-axis range, to adjust it to the scale
		// of the data we are looking at. Since we are displaying spectrums we want
		// it to be in the range [ -80, 12 ] ( dB's )
		lowPassPlot.SetYRange( -80, 12 );

		// And finally we can set the data we want the plot to show. For now we will
		// limit to plot the spectrum magnitude BPF. Since a BPF (break point function )
		// is an implicit way of defining a function, we should also specify the number
		// of samples we want the Plot to take from the BPF
		lowPassPlot.SetData( lowPass.GetMagBPF(), 1000 );

		// Here we make the plot to appear on the screen. Note that your program will 
		// be blocked until you close the plot window. Note that here we use the
		// name we gave to the plot object so to being able to call it come forward
		CLAMVM::SystemPlots::Display( "plot_1" );

		// To note the importance of the number of samples the plot takes from
		// a BPF, let's see what happens when we specify a significantly lower number
		// of samples:
		lowPassPlot.SetData( lowPass.GetMagBPF(), 10 );
		
		CLAMVM::SystemPlots::Display( "plot_1" );

		// Quite blocky, isn't it?

		// Obviously, there is the possibility of showing several plots
		// simultaneously on the screen. So let's first create two more plots
		CLAMVM::Plot  bandPassPlot("plot_2");
		CLAMVM::Plot  highPassPlot("plot_3");

		bandPassPlot.SetLabel( "Band-pass filter frequency response");
		bandPassPlot.SetTooltipFormat( "f=%6.f Hz, H(f)=%3.2f dB" );
		bandPassPlot.SetPosition( 50, 50 );
		bandPassPlot.SetSize( 512, 512 );
		bandPassPlot.SetColor( CLAMVM::Color( 127, 150, 0 ) );
		bandPassPlot.SetYRange( -80, 12 );

		highPassPlot.SetLabel( "High-pass filter frequency response");
		highPassPlot.SetTooltipFormat( "f=%6.f Hz, H(f)=%3.2f dB" );
		highPassPlot.SetPosition( 50, 50 );
		highPassPlot.SetSize( 512, 512 );
		highPassPlot.SetColor( CLAMVM::Color( 127, 150, 0 ) );
		highPassPlot.SetYRange( -80, 12 );

		// And now let's associate each plot with a suitable BPF
		lowPassPlot.SetData( lowPass.GetMagBPF(), 256 );
		bandPassPlot.SetData( bandPass.GetMagBPF(), 256 );
		highPassPlot.SetData( highPass.GetMagBPF(), 256 );

		// Now we have two options: either we show all the plots at the same
		// time by issuing
		CLAMVM::SystemPlots::DisplayAll();

		// Or we just show a few of them, by building a "Display List"
		CLAMVM::SystemPlots::BeginDisplayList();
		CLAMVM::SystemPlots::Display( "plot_3" );
		CLAMVM::SystemPlots::Display( "plot_1" );
		CLAMVM::SystemPlots::EndDisplayList();

		// Note that that in either case your program is blocked until you
		// close all plot windows.

		// Besides BPFs, CLAMVM::Plot objects can also display CLAM::Arrays. To
		// do this we will just use one of the plots we have already created.
		// First, we will measure the randomness of our local rand() standard
		// C library function

		CLAM::Array<CLAM::TData> measurement;

		std::cout << "Generating data...";
		std::cout.flush();

		measureRandRandomness( measurement );

		std::cout << std::endl;

		// Now we will change a few of the previous customizations we made on
		// the lowPass Plot object

		lowPassPlot.SetLabel( "Local rand() randomness" );
		lowPassPlot.SetTooltipFormat( "x=%4.f, P(x)=%1.6f" );
		lowPassPlot.SetYRange( 0.0, 2.0/64.0 );

		// And we will just attach the new data to the lowPass plot. Note
		// that for an Array - the explicit representation of a function
		// of some sort - you don't have to tell the plot the number of
		// samples to take, but the range ( over the real numbers ) where
		// the samples are meant to be distributed. Since we will be generating
		// numbers between 0 and 64 thats the x range for this function
		lowPassPlot.SetData( measurement, 0, 64 );

		CLAMVM::SystemPlots::Display( "plot_1" );

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
	
