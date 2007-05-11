
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
 *      -> Create a CLAM Processing Network with flow control
 *      -> Add Processing Objects to this network
 *      -> Create connections of ports and controls to pass data between them
 *      -> Manage the state of the network
 */

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "Err.hxx"
#include "SimpleOscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AudioOut.hxx"
#include <iostream>
#include "AudioManager.hxx"
#include <CLAM/PANetworkPlayer.hxx>

int main( int argc, char** argv )
{	
	try
	{
		// These values will be used in some configurations, so we declare it now.
		int size = 512;
		int sampleRate = 44100;

		// We need to deploy the audio manager class in order to get audio sound.
		CLAM::AudioManager audioManager( sampleRate, size );

		// First of all we need to create a clam network. It isn't really complicated; just setting the name.
		CLAM::Network network;
		network.SetName("My Network");

		// In order to add processings to the network, we will create correct configurations for all of them.
		// In a lot of cases the default configurations are ok.

		CLAM::SimpleOscillatorConfig osc1Cfg;
		osc1Cfg.SetFrequency(440.0);
		osc1Cfg.SetSamplingRate( sampleRate );

		CLAM::SimpleOscillatorConfig osc2Cfg;
		osc2Cfg.SetFrequency(20.0);
		osc2Cfg.SetSamplingRate( sampleRate );

		CLAM::BinaryAudioOpConfig audioMultiplierCfg;
			
		// Ok, we have the configurations created. Why not to create the processing that will use them?
		// Because we will add them directly to the network, in this way:

		network.AddProcessing( "Generator", new CLAM::SimpleOscillator( osc1Cfg ) );
		network.AddProcessing( "Modulator", new CLAM::SimpleOscillator( osc2Cfg ) );
		network.AddProcessing( "Audio Multiplier", new CLAM::AudioMultiplier( audioMultiplierCfg ) );
		network.AddProcessing( "Audio Out", new CLAM::AudioSink );

		// so we see that for each processing we add, it receives a particular name. We can acces the processings
		// of the network using this name, as we will see with the connections in a few moments.

		// First we connect both oscillators to the audio multiplier.
		network.ConnectPorts( "Generator.Audio Output", "Audio Multiplier.First Audio Input" );
		network.ConnectPorts( "Modulator.Audio Output", "Audio Multiplier.Second Audio Input" );
		// as you can see, the interface is ("processing_producer.out_port", "processing_consumer.in_port").
		// Now the final connection from the multiplier to the audio output:
		network.ConnectPorts( "Audio Multiplier.Audio Output", "Audio Out.AudioIn" );

		network.SetPlayer(new CLAM::PANetworkPlayer );

		network.Start();

		std::cout << "Press return to stop the noise" << std::endl;
		std::cin.get();

		network.Stop();

		std::cout << "ok, now I know how the CLAM network works!" << std::endl;
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

