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
 *      -> Configure Audio devices in your system using CLAM tools
 *      -> Play a WAVE file using CLAM abstraction of your system
 *      -> audio devices
 */
#include "AudioIO.hxx"      // imports CLAM::AudioManager and CLAM::AudioIOConfig declarations
#include "AudioManager.hxx"
#include "AudioOut.hxx"     // imports CLAM::AudioOut declaration
#include "AudioFileIn.hxx"  // this header imports the CLAM::AudioFileIn Processing class interface
#include "Audio.hxx"        // imports the CLAM::Audio ProcessingData class interface
#include "Err.hxx"          // imports CLAM::Err exception class declaration
#include <FL/fl_file_chooser.H>
#include <iostream>


int main( int argc, char** argv )
{
	try
	{
		// First we will load an audio file, just as shown in the AudioFileIO example, so
		// refer to that code in order to get a detailed explanation of the code below
		CLAM::AudioFileIn   fileLoader; 
		CLAM::AudioFileConfig fileLoaderConfig; 
		const char* filename = fl_file_chooser( "Please, select a .wav file", "*.wav", NULL );
		
		if ( filename == NULL )
		{
			std::cout << "User cancelled" << std::endl;
			exit(0);
		}
		
		fileLoaderConfig.SetFilename( filename );
		fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave ); 
		
		if ( !fileLoader.Configure( fileLoaderConfig ) )
		{
			std::cerr << "ERROR: could not open " << filename << std::endl;
			exit(-1);
		}
		
		CLAM::TSize nSamples = fileLoader.Size();
		CLAM::TData sampleRate = fileLoader.SampleRate();

		CLAM::Audio   signalLeftChannel;
		CLAM::Audio   signalRightChannel;
		
		signalLeftChannel.SetSize( nSamples );
		signalLeftChannel.SetSampleRate( sampleRate );

		// Let's check wether the signal is stereo or mono ( it has two channels
		// or just one )
		
		if ( fileLoader.Channels() == 2 )
		{
			signalRightChannel.SetSize( nSamples );
			signalRightChannel.SetSampleRate( sampleRate );
		}
		else if ( fileLoader.Channels() > 2 )
		{
			std::cerr << "Only stereo or mono files playback is supported! " << std::endl;
			exit(-1);
		}
		
		fileLoader.Start();      

		// We load the samples onto one or two Audio objects, as it is appropiate
		if ( fileLoader.Channels() == 1 )
			fileLoader.Do( signalLeftChannel );
		else if ( fileLoader.Channels() == 2 )
			fileLoader.Do( signalLeftChannel, signalRightChannel );
		
		fileLoader.Stop();
		
		// Once we have loaded the sound file, it's time to access and configure the audio hardware devices
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

		
		const CLAM::TData playbackSampleRate = 44100;
		const CLAM::TSize frameSize = 1024;

		CLAM::AudioManager theAudioManager( playbackSampleRate, frameSize );

		// Now we will create two AudioOut objects, one for each channel in our card. Note that even
		// if the sound you are playing is mono, you must write something onto both channels.

		CLAM::AudioOut leftChannel;
		CLAM::AudioOut rightChannel;

		// And now we will configure them
		CLAM::AudioIOConfig  audioOutCfg;
		
		// We set the channel id. Sound devices channels are numbered from
		// [0...N] - being N the number of channels your sound device can
		// support. Usually the channel '0' corresponds to the left channel.
		audioOutCfg.SetChannelID( 0 );
		// and we configure the left channel audio object
		leftChannel.Configure( audioOutCfg );

		// we will use the same config object ( remember that they are copied
		// internally by Processings ) to set the channel ID for the right
		// channel object. usually 'right channel' is numbered as '1'
		audioOutCfg.SetChannelID( 1 );
		// and we configure the other channel audio object
		rightChannel.Configure( audioOutCfg );

		// And we may now start them
		leftChannel.Start();
		rightChannel.Start();
	       
		// Now we can start writing to the sound card. However, things are not
		// so easy as just calling the AudioOut objects Do passing them the
		// Audio objects where we store the samples: we must break these big
		// ones into smaller ones. 
		// So the task at hand can be detailed as follows:
		// 1) Partition the big audios loaded from files into smaller audios
		//    called frames, paying attention to the fact that big audio sizes
		//    might not be exact multiples of the frameSize we chose before
		// 2) Pass these frames to the AudioOut objects so they are played by
		//    our sound card
		// The second one is quite trivial, but the first one require some more
		// thought:
		// i)  What we can do if the big audio size is not a multiple of the frame
		//     size? One reasonable thing to do is to set those 'missing samples'
		//     to zero
		// ii) This problem can be reduced to that of having an sliding window
		//     moving through the big audio. This 'window' can be represented in
		//     a variety of ways, such as a pair of indexes. You will see this
		//     idea of 'sliding window' over a set of data to appear here and there
		//     in CLAM.

		// So let's instantiate two Audio objects for holding the samples meant
		// for each channel at each 'step' of our play back algorithm
		CLAM::Audio leftSamples;
		CLAM::Audio rightSamples;
		// We set their sizes to the 'frameSize' and their sample rates to those
		// of the big audios
		leftSamples.SetSize( frameSize );
		rightSamples.SetSize( frameSize );
		leftSamples.SetSampleRate( sampleRate );
		rightSamples.SetSampleRate( sampleRate );

		// We will implement the idea of keeping our window as 'a pair of indexes'
		CLAM::TIndex leftIndex = 0;
		CLAM::TIndex rightIndex = frameSize;

		// And now let's start the loop
		while ( leftIndex < nSamples )
		{
			// Now it's time to 'extract' the samples bounded by
			// our window from the big audios. This can be accomplished with
			// the GetAudioChunk() method present in Audio objects interface,
			// which is able to solve the 'not multiple of' case
			// Note that depending wether we are handling a mono or a stereo
			// signal, the sources of data for our 'temporary' buffers vary.
			if ( fileLoader.Channels() == 1 )
			{
			
				signalLeftChannel.GetAudioChunk( leftIndex, rightIndex, leftSamples );
				signalLeftChannel.GetAudioChunk( leftIndex, rightIndex, rightSamples );
			}
			else
			{
				signalLeftChannel.GetAudioChunk( leftIndex, rightIndex, leftSamples );
				signalRightChannel.GetAudioChunk( leftIndex, rightIndex, rightSamples );
			}

			// And now we may wrote to the audio device

			leftChannel.Do( leftSamples );
			rightChannel.Do( rightSamples );

			leftIndex += frameSize;
			rightIndex += frameSize;
		}
		
		// During the file playback you may hear 'cracks' or 'drop-outs'. These artifacts
		// are called 'buffer underruns' i.e. that your program has not been fast enough
		// writing to the card - so there are times that the card has no data to play back.
		// This can be a major problem in production code, and its sources are many and varied
		// - under Windows, playing back without having underruns from a pure console
		// application is sort of 'black art' being the frameSize an important factor

		// And finally, we stop the AudioOut's
		leftChannel.Stop();
		rightChannel.Stop();
		
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
