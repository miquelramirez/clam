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

#include <iostream>
#include "Spectrum.hxx"         // imports CLAM::Spectrum declaration
#include "SpectrumConfig.hxx"	// imports CLAM::SpectrumConfig and CLAM::SpecTypeFlags
								// declarations
#include "FFT_rfftw.hxx"        // imports CLAM::FFT_rfftw Processing declaration
#include "XMLStorage.hxx"       // imports CLAM XML interface
#include "DataTypes.hxx"        // imports CLAM-defined types declarations
#include "Err.hxx"              // imports CLAM::Err exception declaration

int main(int argc, char* argv[])
{
	try 
	{
		// In CLAM audio signals are modeled as CLAM::Audio class instances. These
		// 'Audio' objects will contain the actual samples that conform the signal
		// as well as some useful info about those samples, such as the sampling rate.
		// On the other hand, frequency domain representations of audio signals are 
		// modeled as CLAM::Spectrum class instances. These 'Spectrum' objects
		// contain both the magnitude and phase of a signal frequency domain representation
		// in various formats ( cartesian or polar complex numbers, as a pair of floating-point
		// number arrays, etc. ), as well as useful data such as the spectral range.
		// One of the various ways of obtaining a frequency domain representation of
		// an audio signal is applying the Fourier Transform. This algebraic operation
		// is performed by an algorithm with a computational cost of O( n � log( n ) ), known
		// as FFT - Fast Fourier Transform. CLAM borrows the algorithm from FFTW, 
		// a library implementing it in a very efficient way. We have encapsulated
		// the FFTW library implementation of the FFTW algorithm inside a Processing object,
		// CLAM::FFT_rfftw.
		// So transforming an Audio object into a Spectrum object brokes down to just:
		// 1) Obtain the signal from wherever it suits
		// 2) Create an instance of CLAM::Spectrum and configure it
		// 3) Configure and start the FFT_rfftw object
		// 4) Execute the FFT_rfftw::Do method on both the input audio signal and
		//    the output frequency domain representation

		// First of all, let's declare some constants used throughout the
		// example
		const CLAM::TSize samples=1024;      // the number of samples in the audio signal
		const CLAM::TData sampleRate=8000.0; // the sample rate of the audio signal
		const CLAM::TData sineFreq = 400.0;  // frequency of the audio signal sinusoidal
											 // component	

		// For this example we will just synthesize the audio signal.
		CLAM::Audio myaudio;
		
		// We set the number of samples that the signal will have
		myaudio.SetSize(samples);
		
		// Synthesis loop. We use the following formula:
		// x(n) = A0 + A1 * sin( 2*PI*f*(n/sampleRate) )
		// this will yield a signal with exactly one sinusoidal component
		for ( CLAM::TSize i=0; i<samples; i++ )
			myaudio.GetBuffer()[i]=0.625+0.5*sin(2.0*sineFreq*400.0*(((float)i)/sampleRate));

		// We create an instance of CLAM::Spectrum
		CLAM::Spectrum myspectrum;
		// Helper class that defines the spectrum format
		CLAM::SpecTypeFlags specFlags;

		// We want the spectrum to be defined as an array of cartesian ( a+b�i )
		// complex numbers
		specFlags.bComplex=1;
		// we disable the representation consisting in a pair of arrays
		// of floating-point numbers ( magnitude and phase )
		specFlags.bMagPhase = 0;
		// we apply the desired format on the object
		myspectrum.SetType(specFlags);

		// We set the number of spectral samples. This number is given by the
		// fact that since we are just interested in the 'real' part of the
		// spectrum, the number of samples will be N/2 ( half the original signal
		// samples ) plus one: the DC or zero frequency component
		myspectrum.SetSize(samples/2+1);

		// We create the configuration object for the FFT_rfftw Processing. We just
		// need to specify the size ( in samples of the input signal ).
		CLAM::FFTConfig fconfig;
		fconfig.SetName("local.My_beautiful_fft");
		fconfig.SetAudioSize(samples);

		// Processing object creation
		CLAM::FFT_rfftw myfft;

		// we configure the object
		myfft.Configure( fconfig );

		// we start it
		myfft.Start();

		std::cout << "Running object " << std::endl;
		// we apply the FFT algorithm on 'myaudio' and leave the result on
		// 'myspectrum'
		myfft.Do( myaudio, myspectrum );

		std::cout << "Storing spectrum"<< std::endl;
		
		// To obtain a persistent copy of the myspectrum object we first instantiate
		// a CLAM::XMLStorage object - this copy will be encoded as a well-formed XML
		// document
		CLAM::XMLStorage storage;

		// and then we tell it to dump the object, setting the 'root' element name of the
		// resulting XML document to 'FFTResult' on the file 'FFT_example.xml'
		storage.Dump(myspectrum, "FFTResult", "FFT_example.xml");

	}
	catch(CLAM::Err& err) // first we check for CLAM internal exceptions
	{
		err.Print();
		exit(-1);
	}
	catch( std::exception& e ) // and then for standard library exceptions
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	return 0;

}
