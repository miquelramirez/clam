/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

// Welcome to the "AudioFileWriting" example. In this example we will show you
// how to use CLAM::MultiChannelAudioFileWriter, a Processing that allows you
// to write simultaneously several channels at once from the same file.

// Before attempting to follow this example you should take a look first on the
// "FileInfo" example ( examples/AudioFileWriting_example.cxx ) and the
// "AudioFileReading" example ( examples/AudioFileReading_example.cxx ).

// We need to include the following files to have access to classes
// AudioFile, MultiChannelAudioFileWriter and MultiChannelAudioFileWriterConfig
#include "AudioFile.hxx"
#include "MultiChannelAudioFileWriter.hxx"

// We also need the declaration of the CLAM::Audio object
#include "Audio.hxx"

// For accessing <cmath> header in a cross-platform way
#include "CLAM_Math.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"

#include <vector>

#include <iostream>

int main( int argc, char** argv )
{
	// As we did in the FileInfo example we will take the output file name from
	// the command line.

	if ( argc == 1 ) // No input file
	{
		std::cerr << "No input file" << std::endl;
		std::cerr << "Usage: AudioFileWriting <output file name>" << std::endl;

		exit( - 1 );
	}
	else if ( argc > 2 ) // Too many parameters
	{
		std::cerr << "Too many parameters" << std::endl;
		std::cerr << "Usage: AudioFileWriting <output file name>" << std::endl;
		
		exit( -1 );
	}
	// As we can be quite sure that we have one parameter at argv[1].
	// Lets prepare the configuration for our file writer.
	const CLAM::TData sampleRate = 44100.; // Hz
	const CLAM::TSize nChannels = 2;
	CLAM::MultiChannelAudioFileWriterConfig configObject;
	// File format (ogg, wav, aiff...) is deduced from the file name extension.
	configObject.SetTargetFile( argv[1] );
	// This is an example on how to change the number of channels
	// and the sample rate. 
	// As we are using the defaults (2 channels and 44100 Hz)
	// we don't need to do it but just as an example on how  to do it.
	configObject.SetSampleRate( sampleRate );
	configObject.SetNChannels( nChannels );

	// Now we can instantiate the MultiChannelAudioFileWriter and configure it
	CLAM::MultiChannelAudioFileWriter writer;
	if ( ! writer.Configure( configObject ) )
	{
		std::cerr << writer.GetConfigErrorMessage() << std::endl;
		exit(-1);
	}

	// Finally, we must set the data we want to write on to the file. We will just implement
	// a simple sinusoidal synthesis, where in each channel there is a sinusoid with the same
	// frequency but different phase

	const CLAM::TData sineFreq = 440.; // Hz
	// initial phases
	CLAM::TData phi[] = { -M_PI / 2.0, M_PI/2.0 };

	// Delta phase: this trick allows us to compute incrementally current sine
	// phase
	CLAM::TData dphi[] = { ( 2.0 * M_PI * sineFreq ) / sampleRate,  ( 2.0 * M_PI * sineFreq ) / sampleRate};

	// Now we must decide our 'write size', i.e. the number of samples we will write
	// at once at the same channel
	const CLAM::TSize writeSize = 2048; // 0.0464 s

	// And now we set our 'input buffers', where we will be generating the sines
	
	std::vector< CLAM::Audio > inputs;
	inputs.resize( nChannels );         // As many inputs as channels

	for ( unsigned i = 0; i < inputs.size(); i++ )
	{
		inputs[i].SetSize( writeSize );
	}

	// Now we want to synthesize approx. 1s

	writer.Start();
	
	for ( int i = 0; i < 25; i++ )
	{
		for ( unsigned int j = 0; j < inputs.size(); j++ )
			for ( int k = 0; k < inputs[j].GetSize(); k++ )
			{
				inputs[j].GetBuffer()[k] = 0.5 * cos( phi[j] );
				phi[j] += dphi[j];
			}

		// Once both input buffers have been generated we call
		// the writer Do()
		writer.Do( inputs );
	}
	
	writer.Stop();

	return 0;
}

