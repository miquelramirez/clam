
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
 *      -> Use network methods to store its content definition in XML.
 *      -> Activate an XML to get a fully functional network again.
 */

#include "XMLStorage.hxx"
#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "Err.hxx"
#include "SimpleOscillator.hxx"
#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AudioAdder.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "AudioFile.hxx"
#include <iostream>
#include <QtGui/QFileDialog>
#include <QtCore/QString>
#include <QApplication>

int main( int argc, char** argv )
{	
	QApplication a(argc,argv);
	try
	{
 		// The first steps of this example are equals to NetworkUsage_example.cxx, so please
		// take a look on it before starting with this, in order to get a full idea about
		// the things we are doing here.
		// Also AudioFileIO_example.cxx should be consulted before.

		int size = 512;
		int sampleRate = 44100;

		// network initialization

		CLAM::AudioManager audioManager( sampleRate, size );
		CLAM::Network * network = new CLAM::Network;
		network->SetName("Network To Store");

		CLAM::SimpleOscillatorConfig osc1Cfg;
		osc1Cfg.SetFrequency(220.0);
		osc1Cfg.SetSamplingRate( sampleRate );

		CLAM::SimpleOscillatorConfig osc2Cfg;
		osc2Cfg.SetFrequency(10.0);
		osc2Cfg.SetPhase(0.5);
		osc2Cfg.SetAmplitude(0.5);
		osc2Cfg.SetSamplingRate( sampleRate );

		CLAM::SimpleOscillatorConfig osc3Cfg;
		osc3Cfg.SetFrequency(880.0);		
		osc3Cfg.SetAmplitude(0.5);
		osc3Cfg.SetSamplingRate( sampleRate );

		CLAM::OscillatorConfig osc4Cfg;
		osc4Cfg.SetFrequency(440.0);
		

		CLAM::AudioIOConfig audioOutCfg;
		audioOutCfg.SetFrameSize( size );
		audioOutCfg.SetSampleRate( sampleRate );
		audioOutCfg.SetChannelID( 0 );

		network->AddProcessing( "Generator", new CLAM::SimpleOscillator( osc1Cfg ) );
		network->AddProcessing( "Modulated Generator", new CLAM::Oscillator( osc4Cfg ) );
		network->AddProcessing( "Phase Modulator", new CLAM::SimpleOscillator( osc2Cfg ) );
		network->AddProcessing( "Frequency Modulator", new CLAM::SimpleOscillator( osc3Cfg ) );
		network->AddProcessing( "multiplier", new CLAM::AudioMultiplier );
		network->AddProcessing( "audio out", new CLAM::AudioOut( audioOutCfg ) );

		network->ConnectPorts( "Generator.Audio Output", "multiplier.First Audio Input" );
		network->ConnectPorts( "Modulated Generator.Audio Output", "multiplier.Second Audio Input" );
		network->ConnectPorts( "multiplier.Audio Output", "audio out.Audio Input" );
		network->ConnectPorts( "Phase Modulator.Audio Output", "Modulated Generator.Input Phase Modulation" );
		network->ConnectPorts( "Frequency Modulator.Audio Output", "Modulated Generator.Input Frequency Modulation" );

		// Now that we have the network created with our desired processing and connections, we will store it to an xml file.

		QString networkFileName = QFileDialog::getSaveFileName(0,
			QObject::tr("Please, specify the xml where network will be stored"),
			QObject::tr("MyNetwork.clamnetwork"),
			"*.clamnetwork");
		if ( networkFileName == NULL )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}

		// the first step to passivate the network is create an storage object
		CLAM::XMLStorage::Dump( *network, "network", networkFileName.toStdString().c_str() );

		// ok, we just stored it. You can take a look to the file you specified, read the contents and modify it if you want
		// (to your own risk, of course).
		// We don't need this network anymore
		delete network;
	
		// It's time to load this network to another instance and check if it works correctly.

		CLAM::Network * network2 = new CLAM::Network; 
		network2->SetName("Network Loaded");

		CLAM::XMLStorage::Restore( *network2, networkFileName.toStdString().c_str() );

		// With these few lines we have in "network2" the same connections and processings (with his configs) that we had in "network".

		network2->Start();

		for(int i=0;i<300;i++)
			network2->Do();

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

