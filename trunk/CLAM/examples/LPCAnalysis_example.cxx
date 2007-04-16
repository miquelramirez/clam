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

#include "LPC_AutoCorrelation.hxx" // imports CLAM LPC_AutoCorrelation Processing declaration
#include "FFT.hxx"           // imports CLAM::FFT Processing declaration
#include "LPModel.hxx"             // imports CLAM::LPModel ProcessingData declaration
#include "Audio.hxx"               // imports CLAM::Audio ProcessingData declaration
#include "Spectrum.hxx"            // imports CLAM::Spectrum ProcessingData declaration
#include "SpectrumConfig.hxx"      // imports CLAM::Spectrum auxiliar datatypes declaration
#include "WindowGenerator.hxx"     // imports CLAM::WindowGenerator Processing declaration
#include "AudioMultiplier.hxx"     // imports CLAM::AudioMultiplier Processing declaration
//#include "MultiPlot.hxx"           // imports CLAMVM::MultiPlot declaration
//#include "SystemPlots.hxx"         // imports CLAMVM::SystemPlots declaration
#include "Err.hxx"                 // imports CLAM::Err declaration
#include <iostream>

int main( int argc, char** argv )
{
	try // note the exception handling
	{
		// In this example we will learn how to do the following things
		// 1) How to generate an analysis window for performing spectral analysis
		// 2) How to use one kind of spectral approximation algorithm: the LPC
		// and finally, just for fun, we will compute the FT of the same windowed
		// audio analyzed by the LPC algorithm, and watch the differences

		// The first thing we need is some Audio data to analyze, so we will synthesize
		// some simple but non trivial signal
		const CLAM::TSize samples=1024;      // the number of samples in the audio signal
		const CLAM::TData sampleRate=44100.0; // the sample rate of the audio signal
		const CLAM::TData sineFreq = 400.0;  // frequency of the audio signal sinusoidal
		const CLAM::TData offsetFreq = 1200.0;
		
		CLAM::Audio myaudio;
		myaudio.SetSize(samples);
		myaudio.SetSampleRate( sampleRate );

		// Not many bells and whistles here - plain old good sinusoidal synthesis
		for ( CLAM::TSize i=0; i<samples; i++ )
			myaudio.GetBuffer()[i]=
				0.1 + 
				0.5*sin(2.0*PI*sineFreq*(((float)i)/sampleRate)) + 
				0.4*sin(2.0*PI*(sineFreq+offsetFreq)*(((float)i)/sampleRate)) +
				0.3*sin(2.0*PI*(sineFreq+(2.0*offsetFreq))*(((float)i)/sampleRate)) +
				0.2*sin(2.0*PI*(sineFreq+(3.0*offsetFreq))*(((float)i)/sampleRate)) +
				0.1*sin(2.0*PI*(sineFreq+(4.0*offsetFreq))*(((float)i)/sampleRate));

		
		// And the next thing we need is to generate the windowing function. Analysis windows
		// in CLAM are just Audio's - at the end they are also signals
		CLAM::Audio window;
		// we set the window size to match that of the signal we just generated
		window.SetSize( samples );

		// We instantiate a CLAM::WindowGeneratorConfig object that we will
		// use for configuring the WindowGenerator Processing1
		CLAM::WindowGeneratorConfig wndGenCfg;
		// We set the kind of function we want to generate. For this example we will
		// use a Blackman-Harris window function with 92dB difference between main and
		// side lobes
		wndGenCfg.SetType( CLAM::EWindowType::eBlackmanHarris92);
		// We set the size of generated windows
		wndGenCfg.SetSize( samples );
		// We also want the WindowGenerator to normalize generated windows
		// as it is meant for spectral analysis 
		wndGenCfg.SetNormalize( CLAM::EWindowNormalize::eAnalysis );

		// We instantiate a WindowGenerator processing
		CLAM::WindowGenerator winGen;

		// We configure it...
		winGen.Configure( wndGenCfg );
		// We start it...
		winGen.Start();
		// And we generate the window calling wingen's Do() method
		// passing as well the Audio we created above for holding the
		// window function
		winGen.Do( window );
		// And we stop the processing...
		winGen.Stop();

		// Third thing: now we must apply the window onto the audio signal
		// we want to get analyzed by multiplying the signal by the window
		// we instantiate the object we will use for holding the 'windowed'
		// signal
		CLAM::Audio windowedAudio;
		windowedAudio.SetSize( samples );

		// The thing that 'multiplies' Audio objects, is obviously enough,
		// a Processing called AudioMultiplier
		CLAM::AudioMultiplier windowApplier;

		// Start, Do and Stop. Note that the parameters in the form
		// myAudio · window = windowedAudio
		windowApplier.Start();
		windowApplier.Do( myaudio, window, windowedAudio );
		windowApplier.Stop();

		// Well, once we have done this essential pre-processing we can try
		// to analyze the audio signal. First we will create an object that
		// will hold the approximation of signal spectrum - a LPModel instance
			       
		CLAM::LPModel lpModel;
		// We will try our hand with a polynomial approximation of 
		// the eleventh order
		const unsigned lpcOrder = 11;
		// We tell the lpSpectrum object which is the order of the
		// approximation we have chosen
		lpModel.UpdateModelOrder( lpcOrder );
		// and we will set the model spectral range
		lpModel.SetSpectralRange( sampleRate / 2 );

		// Now it's time to create the LPC_AutoCorrelation Processing config
		// object
		CLAM::LPCConfig cfg;
		// We set the order of the approximation to be computed to the same
		// we did for the LPModel object
		cfg.SetOrder( lpcOrder );

		// We instantiate the processing
		CLAM::LPC_AutoCorrelation lpc;

		// We configure it...
		lpc.Configure( cfg );
		// start ...
		lpc.Start();
		// call its Do() passing the windowed audio data and the LPModel
		// object where the approximation is to be stored
		lpc.Do( windowedAudio, lpModel );
		// ... and we finally stop the object
		lpc.Stop();

		// We setup the flags for the Spectrum representation of the
		// information contrived in the LPModel object
		CLAM::SpecTypeFlags flags;
		// since we just want a Magnitude/Phase floating-point arrays
		// representation we deactivate the other representations
		flags.bMagPhase=1;
		flags.bComplex = 0;

		// We instantiate here the Spectrum object we will use to reflect
		// LPModel computed information
		CLAM::Spectrum lpSpectrum;
		lpSpectrum.SetSize( samples/2+1 );
		lpSpectrum.SetSpectralRange( sampleRate/2 );
		lpSpectrum.SetType( flags );

		// We tell the LPModel to transfer its information to the Spectrum
		lpModel.ToSpectrum( lpSpectrum );
		// We make the magnitude buffer in the Spectrum to be set in logarithmic
		// scale
		lpSpectrum.ToDB();
		
		// And now it is FFT turn. For details on the following lines refer to
		// FFT_Example.cxx file, where they are explained in depth and detail
		CLAM::Spectrum ftSpectrum;
		ftSpectrum.SetSize( samples/2+1 );
		ftSpectrum.SetSpectralRange( sampleRate/2 );
		ftSpectrum.SetType( flags );
		
		CLAM::FFTConfig fconfig;
		fconfig.SetAudioSize(samples);
		CLAM::FFT myfft;
		myfft.Configure( fconfig );
		myfft.Start();
		myfft.Do( windowedAudio, ftSpectrum );
		myfft.Stop();
		ftSpectrum.ToDB();
/*
 		// TODO: Port this to qt4

		// Now we create the MultiPlot that will hold the drawings
		// of the spectrum we have obtained - refer to MultiPlot_example.cxx
		// for a detailed explanation on this
		CLAMVM::MultiPlot  thePlot( "multiplot" );
		thePlot.SetLabel( "LP spectrum (white) vs. FT spectrum (green)" );
		thePlot.SetPosition( 100, 100 );
		thePlot.SetSize( 640, 480 );
		thePlot.SetYRange( -120, 20 );
		thePlot.SetColor( "LP spectrum", CLAMVM::Color( 200, 200, 200 ) );
		thePlot.AddData( "LP spectrum", lpSpectrum.GetMagBuffer(), 0, 22050 );
		thePlot.SetColor( "FT spectrum", CLAMVM::Color( 0, 200, 0 ) );
		thePlot.AddData( "FT spectrum", ftSpectrum.GetMagBuffer(), 0, 22050 );
		
		CLAMVM::SystemPlots::Display( "multiplot" );
*/
	}
	catch( CLAM::Err& e ) // we handle here CLAM exceptions
	{
		e.Print();
		exit(-1);
	}
	catch( std::exception& e ) // we handle here standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		exit( -1 );
	}

	return 0;
}

