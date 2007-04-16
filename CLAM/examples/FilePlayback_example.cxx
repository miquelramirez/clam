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
 *      -> Configure Audio devices in your system using CLAM tools
 *      -> Play a file using CLAM abstraction of audio devices
 *
 */
#include "AudioIO.hxx"      // imports CLAM::AudioManager and CLAM::AudioIOConfig declarations
#include "AudioManager.hxx"
#include "AudioOut.hxx"     // imports CLAM::AudioOut declaration
#include "AudioFile.hxx"    // imports CLAM::AudioFile declaration
#include "MultiChannelAudioFileReader.hxx" // imports CLAM::MultiChannelAudioFileReader
#include "Audio.hxx"        // imports the CLAM::Audio ProcessingData class interface
#include "Err.hxx"          // imports CLAM::Err exception class declaration

#include <iostream>


int main( int argc, char** argv )
{
	try
	{
		// We take as a parameter from the command line the file to be played back
		
		if ( argc == 1 ) // No input file
		{
			std::cerr << "No input file" << std::endl;
			std::cerr << "Usage: AudioFileReading <input file name>" << std::endl;
			
			exit( - 1 );
		}
		else if ( argc > 2 ) // Too many parameters
		{
			std::cerr << "Too many parameters" << std::endl;
			std::cerr << "Usage: AudioFileReading <input file name>" << std::endl;
			
			exit( -1 );			
		}
	
		// Now we will prepare file playback very similarly as the first half of the
		// "AudioFileReading" example

		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile( argv[1] );
		CLAM::MultiChannelAudioFileReader reader;
		if (!reader.Configure( cfg ))
		{
			std::cerr << "Error: file " << cfg.GetSourceFile() << " cannot be opened ";
			std::cerr << "or is encoded in an unrecognized format" << std::endl;
			exit(-1);
		}

		std::vector<CLAM::Audio> incomingAudioChannels;
		incomingAudioChannels.resize( reader.GetAudioFile().GetHeader().GetChannels() );

		// Once we made sure we can start playing the file, it's time to access and configure the audio hardware devices
		// present in our computer. We will do this through the 'AudioManager': a global, unique object that
		// mediates between your program and the underlying Operating System sound hardware services.
		
		// So we first create an instance of AudioManager by passing its constructor two 
		// values: the first one being the sample rate we want the card to play the samples 
		// we write onto it and thenumber of samples we will want to write on to the
		// card at once. 
		// In this example we will try 44.1 kHz playback sample rate, and we will estimate that 1024
		// will be well enough for our purposes.
		// The AudioManager class should be declared before AudioOut objects, because
		// they need this manager to know how to play the sound (which sound device is available for them).

		// NOTE about the frame size determination: The size of these 'frames' can be anything
		// reasonable below the maximum buffer size we told the AudioManager
		// enforce: 0 or 3 frame sizes are *not* reasonable. For this example
		// we will use as frame size, 1024, which is a nice number for a variety
		// of reasons, not being the only one the fact it is a power of two :)

		
		const CLAM::TData playbackSampleRate = reader.GetAudioFile().GetHeader().GetSampleRate();
		const CLAM::TSize frameSize = 1024; // this is also our read size

		// And now we set the size of each Audio object to our intended 'read size'
		// in this case ouf frame size
		for ( unsigned i = 0; i < incomingAudioChannels.size(); i++ )
		{
			incomingAudioChannels[i].SetSize( frameSize );
		}

		CLAM::AudioManager theAudioManager( (int)playbackSampleRate, frameSize );

		// Now we will create as many AudioOuts as input channels

		CLAM::Array<CLAM::AudioOut > audioOutputs;
		audioOutputs.Resize( incomingAudioChannels.size() );
		audioOutputs.SetSize( incomingAudioChannels.size() );

		// And now we will configure them
		CLAM::AudioIOConfig  audioOutCfg;

		for ( int i = 0; i < audioOutputs.Size(); i++ )
		{		
			// We set the channel id. Sound devices channels are numbered from
			// [0...N] - being N the number of channels your sound device can
			// support. Usually the channel '0' corresponds to the left channel.
			audioOutCfg.SetChannelID( i );
			// and we configure the left channel audio object
			audioOutputs[i].Configure( audioOutCfg );
			// We start it
			audioOutputs[i].Start();
		}

		reader.Start();
		
		while( reader.Do( incomingAudioChannels ) )
		{
			for ( int i = 0; i < audioOutputs.Size(); i++ )
				audioOutputs[i].Do( incomingAudioChannels[i] );
		}

		
		for ( int i = 0; i < audioOutputs.Size(); i++ )
			audioOutputs[i].Stop();

		reader.Stop();
		
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

