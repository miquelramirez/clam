
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


/**
 *  The goals of this example is to show you how to:
 *      -> Create a CLAM Processing with Ports and Controls
 *      -> Connect them to pass control messages and data to process
 *      -> Illustrate how to use Do() instead of the version with parameters.
 */

#include <iostream>
#include "Err.hxx"
#include "SimpleOscillator.hxx"
#include "MonoAudioFileWriter.hxx"
#include "AudioFile.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "FFT.hxx"
#include "IFFT.hxx"

int main( int argc, char** argv )
{	
	try
	{
		// we won't comment the code related to audio I/O and processing
		// configuration, because they are questions discussed
		// in another examples (like FilePlayback_example.cxx 
		// and ProcessingLifeCycle_example.cxx).
		int sampleRate = 44100;
		int size = 256;

		CLAM::AudioManager manager(sampleRate, size);
		manager.Start();

		// In this example, we will produce a sine signal with an oscillator,
		// convert it into frequency domain spectrum with an FFT (Fast Fourier
		// Transform), way back to audio with an IFFT (Inverse FFT), then
		// the output is both sent to the speakers and an ouput file.
		CLAM::SimpleOscillator osc;
	
		CLAM::FFTConfig fconfig;
		fconfig.SetAudioSize(size);	
		CLAM::FFT myfft( fconfig );

		CLAM::IFFTConfig ifconfig;
		ifconfig.SetAudioSize(size);
		CLAM::IFFT myifft( ifconfig );
	
		CLAM::MonoAudioFileWriterConfig configWriter;
		configWriter.SetTargetFile( "test.wav" );
		CLAM::MonoAudioFileWriter writer( configWriter );

		CLAM::AudioOut audioOut;

		// If we configure audio ports the same size than the FFT
		// then all the processings can be run at the same rate
		// and there is no need of a complex flow control.
		osc.GetOutPort("Audio Output").SetSize( size );
		osc.GetOutPort("Audio Output").SetHop( size );

		audioOut.GetInPort("Audio Input").SetSize( size );
		audioOut.GetInPort("Audio Input").SetHop( size );

		writer.GetInPort("Samples Write").SetSize( size );
		writer.GetInPort("Samples Write").SetHop( size );

		// Connecting the ports of each processing
		CLAM::ConnectPorts( osc, "Audio Output", myfft, "Audio Input");
		CLAM::ConnectPorts( myfft, "Spectrum Output", myifft, "Spectrum Input");
		CLAM::ConnectPorts( myifft, "Audio Output", audioOut, "Audio Input");
		CLAM::ConnectPorts( myifft, "Audio Output", writer, "Samples Write");

	
		osc.Start();
		myifft.Start();
		myfft.Start();
		writer.Start();
		audioOut.Start();


		for(int i=0;i<500;i++)
		{
			osc.Do();
			myfft.Do();
			myifft.Do();
			writer.Do();
			audioOut.Do();
		}

		osc.Stop();
		myfft.Stop();
		myifft.Stop();
		writer.Stop();
		audioOut.Stop();
		
		std::cout << "ok, now I know how to use CLAM ports!" << std::endl;
	}
	catch ( CLAM::Err& e )
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
