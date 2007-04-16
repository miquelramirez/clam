
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
#include "AutoPanner.hxx"
#include "SimpleOscillator.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "Audio.hxx"

#include "MyProcessingWithPortsAndControls.hxx" // this is our new processing, created with ports and controls

int main( int argc, char** argv )
{	
	try
	{
		// we won't comment the code related to audio I/O and processing configuration, because they are questions discussed
		// in another examples (like FilePlayback_example.cxx  and ProcessingLifeCycle_example.cxx).

		int size = 512;
		int samplerate = 44100;

		CLAM::AudioManager manager(samplerate, size);
		manager.Start();

		// In this example, we will produce a sine signal with an oscillator. The data produced will be 
		// processed by our new processing (see MyProcessingWithPortsAndControls.hxx/cxx for more details), and finally
		// passed to the audio out. Our processing will have control signals from autopanner (that sends an LFO signal).

		CLAM::SimpleOscillator osc;
		CLAM::AutoPanner autoPanner;
		CLAM::AudioOut audioOut;
		CLAM::MyProcessingWithPortsAndControls myProc;

		// we need to configure the ports with the correct size in order to get the data automatically.
		// the way of accessing the ports is by name, specifying before which kind of port you need.
		osc.GetOutPort("Audio Output").SetSize( size );
		osc.GetOutPort("Audio Output").SetHop( size );
		audioOut.GetInPort("Audio Input").SetSize( size );
		audioOut.GetInPort("Audio Input").SetHop( size );
		myProc.GetInPort("Audio Input").SetSize( size );
		myProc.GetInPort("Audio Input").SetHop( size );
		myProc.GetOutPort("Audio Output").SetSize( size );
		myProc.GetOutPort("Audio Output").SetHop( size );
		

		// after this, is needed to connect the different ports
		CLAM::ConnectPorts(osc, "Audio Output", myProc, "Audio Input");
		CLAM::ConnectPorts(myProc, "Audio Output", audioOut, "Audio Input");

		// we link one of the out controls of autopanner to our processing, too.
		CLAM::ConnectControls(autoPanner, "Left Control", myProc, "Modulation" );

		// this is the easy part: after the configuration, we can start, 
		// call do and stop the processings without any complication
		// related to intermediate data.

		autoPanner.Start();
		osc.Start();
		myProc.Start();
		audioOut.Start();

		for(int i=0;i<500;i++)
		{
			autoPanner.Do();
			osc.Do();
			myProc.Do();
			audioOut.Do();
		}

		autoPanner.Stop();
		osc.Stop();
		myProc.Stop();
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

