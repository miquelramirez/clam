
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


/**
 *  The goals of this example is to show you how to:
 *      -> Use network methods to store its content definition in XML.
 *      -> Activate an XML to get a fully functional network again.
 */

#include "XMLStorage.hxx"
#include "Network.hxx"
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "Err.hxx"
#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"
#include "AudioFileIn.hxx" 
#include "AudioFileOut.hxx" 
#include "AudioFile.hxx"
#include <iostream>
#include <FL/fl_file_chooser.H>

int main( int argc, char** argv )
{	
	try
	{
 		// The first steps of this example are equals to NetworkUsage_example.cxx, so please
		// take a look on it before starting with this, in order to get a full idea about
		// the things we are doing here.
		// Also AudioFileIO_example.cxx should be consulted before.

		int size = 512;
		int sampleRate = 44100;

		// we will let the user choose file names for loading and saving audio.
		
		const char* fileName = fl_file_chooser( "Please, select a mono .wav file sampled to 44100 Hz", "*.wav", NULL );
		if ( fileName == NULL )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}

		CLAM::AudioFileConfig audioFileInCfg;
		audioFileInCfg.SetKeepFrameSizes( true );
		audioFileInCfg.SetFilename( fileName );
		audioFileInCfg.SetFrameSize( size );
		audioFileInCfg.SetFiletype( CLAM::EAudioFileType::eWave );
		
		const char* outputFileName = fl_file_chooser(  "Please, specify the wav where result will be stored", "*.wav", NULL );
		if ( outputFileName == NULL )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}
			
		CLAM::AudioFileConfig audioFileOutCfg;
		audioFileOutCfg.SetFilename( outputFileName );
		audioFileOutCfg.SetKeepFrameSizes( true );
		audioFileOutCfg.SetFrameSize( size );
		audioFileOutCfg.SetSampleRate( sampleRate );
		audioFileOutCfg.SetFiletype( CLAM::EAudioFileType::eWave );


		// network initialization

		CLAM::AudioManager audioManager( sampleRate, size );
		CLAM::Network * network = new CLAM::Network;
		network->SetName("Network To Store");
		network->AddFlowControl( new CLAM::PushFlowControl( size ));


		CLAM::OscillatorConfig osc1Cfg;
		osc1Cfg.SetFrequency(440.0);
		osc1Cfg.SetSamplingRate( sampleRate );

		CLAM::OscillatorConfig osc2Cfg;
		osc2Cfg.SetFrequency(40.0);
		osc1Cfg.SetSamplingRate( sampleRate );

		CLAM::BinaryAudioOpConfig audioMultiplierCfg;

		CLAM::BinaryAudioOpConfig audioAdderCfg;
			
		CLAM::AudioIOConfig audioOutCfg;
		audioOutCfg.SetFrameSize( size );
		audioOutCfg.SetSampleRate( sampleRate );
		audioOutCfg.SetChannelID( 0 );

		// in this example we will have an audio file signal added to an oscillator, the result
		// multiplied in order to modulate it. Finally we will store the signal on another 
		// audio file while we hear it.

		network->AddProcessing( "File In", new CLAM::AudioFileIn( audioFileInCfg ) );
		network->AddProcessing( "Generator", new CLAM::Oscillator( osc1Cfg ) );
		network->AddProcessing( "Modulator", new CLAM::Oscillator( osc2Cfg ) );
		network->AddProcessing( "Audio Multiplier", new CLAM::AudioMultiplier( audioMultiplierCfg ) );
		network->AddProcessing( "Audio Adder", new CLAM::AudioAdder( audioAdderCfg ) );
		network->AddProcessing( "Audio Mono Out", new CLAM::AudioOut( audioOutCfg ) );
		network->AddProcessing( "File Out", new CLAM::AudioFileOut( audioFileOutCfg ) );

		network->ConnectPorts( "Generator.Audio Output", "Audio Adder.First Audio Input" );
		network->ConnectPorts( "File In.Output", "Audio Adder.Second Audio Input" );
		network->ConnectPorts( "Audio Adder.Audio Output", "Audio Multiplier.First Audio Input" );
		network->ConnectPorts( "Modulator.Audio Output", "Audio Multiplier.Second Audio Input" );
		network->ConnectPorts( "Audio Multiplier.Audio Output", "Audio Mono Out.Input" );
		network->ConnectPorts( "Audio Multiplier.Audio Output", "File Out.Input" );

		// Now that we have the network created with our desired processing and connections, we will store it to an xml file.

		const char* networkFileName = fl_file_chooser(  "Please, specify the xml where network will be stored", "*.xml", NULL );
		if ( networkFileName == NULL )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}

		// the first step to passivate the network is create an storage object
		CLAM::XMLStorage savingObject;
		// now we can store the network to the file choosed before. Before this step, we set indentation to create a file user-readable
		savingObject.UseIndentation( true );
		savingObject.Dump( *network, "network", networkFileName );

		// ok, we just stored it. You can take a look to the file you specified, read the contents and modify it if you want
		// (to your own risk, of course).
		// We don't need this network anymore
		delete network;
	
		// It's time to load this network to another instance and check if it works correctly.

		CLAM::Network * network2 = new CLAM::Network; 
		network2->SetName("Network Loaded");
		network2->AddFlowControl( new CLAM::PushFlowControl( size ));

		CLAM::XMLStorage loadingObject;
		loadingObject.Restore( *network2, outputFileName );

		// With these few lines we have in "network2" the same connections and processings (with his configs) that we had in "network".

		network2->Start();

		for(int i=0;i<300;i++)
			network2->DoProcessings();

		network2->Stop();

		delete network2;
		std::cout << "ok, now I know how to passivate and activate a CLAM network!" << std::endl;

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
