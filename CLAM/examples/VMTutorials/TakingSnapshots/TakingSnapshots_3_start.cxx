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
#include <exception>
#include <string>
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "FFT_rfftw.hxx"
#include "Err.hxx"

void setupAudioSignal( CLAM::Audio& audioObj, CLAM::TData sampleRate, CLAM::TSize size )
{
	audioObj.SetSize( size );
	audioObj.SetSampleRate( sampleRate );

	for (int i=0; i < size;i++)
		audioObj.GetBuffer()[i]=0.5*sin(2.0*PI*400.0*(((float)i)/sampleRate));
}

void setupSpectrum( CLAM::Spectrum& specObj, CLAM::TSize size )
{
	// Spectrum attribute selection and config
	CLAM::SpecTypeFlags sflags;
	sflags.bMagPhase=true;
	sflags.bComplex=true;
		
	CLAM::SpectrumConfig sconfig;
	sconfig.SetType(sflags);
	sconfig.SetSize(size/2+1);	
	
	specObj.Configure( sconfig );
}

void setupFFT( CLAM::FFT_rfftw& fftObj, CLAM::TSize size )
{
	CLAM::FFTConfig fconfig;
	fconfig.SetName(std::string("local.My_beautiful_fft"));
	fconfig.SetAudioSize(size);

	fftObj.Configure( fconfig );
}

int main( int argc, char** argv )
{
	try
	{
		CLAM::TSize size = 1024;
		CLAM::TData sampleRate = 8000.0;

		CLAM::Audio     input;
		CLAM::Spectrum  output;
		CLAM::FFT_rfftw fft;       

		setupAudioSignal( input, sampleRate, size );
		setupSpectrum( output, size );
		setupFFT( fft, size );
		
		fft.Start();
		
		fft.Do( input, output );

		std::cout << "Fourier Transform done!" << std::endl;

		fft.Stop();
	}
	catch( CLAM::Err& e )
	{
		std::cerr << "CLAM exception caught:" << std::endl;
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << "Standard Library exception caught" << std::endl;
		std::cerr << e.what() << std::endl;
		exit(-1);
	}
	catch( ... )
	{
		std::cerr << "Unknown exception caught" << std::endl;
		exit(-1);
	}
	
	return 0;
}

