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

// Welcome to the "AudioFileReading" example. In this example, we will show you
// how to use CLAM::MultiChannelAudioFileReader, a Processing that allows to read
// simultaneously several channels at once from the same file.
//
// Before attempting to follow this example you should take a look first on
// the "FileInfo" example ( examples/FileInfo_example.cxx )

// We must include these headers to have access to classes
// AudioFile, MultiChannelAudioFileReader and MultiChannelAudioFileReaderConfig
#include <iostream>
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReader.hxx"
// Also we need the declaration of CLAM::Audio object, the object which
// represents an ( or portion of an ) audio signal
#include "Audio.hxx"
// For accessing <cmath> header in a cross-platform way
#include "CLAM_Math.hxx"
#include "DataTypes.hxx"

#include <vector>

int main( int argc, char** argv )
{
	// As we did in the FileInfo example we will take the input file from
	// the command line.

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
	
	// Now we are pretty sure that argv[1] holds a parameter.
	// We create a configuration for the MultiChannelAudioFileReader,
	// which will read the samples from the file
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	// We set the 'SourceFile' config attribute to the parameter we received
	cfg.SetSourceFile( argv[1] );
	// Intantiate the processing using such configuration
	CLAM::MultiChannelAudioFileReader reader( cfg );

	// And now check that the given file can be read
	if ( !reader.GetAudioFile().IsReadable() )
	{
		std::cerr << "Error: file " << cfg.GetSourceFile() << " cannot be opened ";
		std::cerr << "or is encoded in an unrecognized format" << std::endl;
		exit(-1);
	}

	// First of all, we must decide which will be our 'read size' i.e. the number
	// of samples we will want to get from each channel at once.
	const CLAM::TSize readSize = 1024;

	// And now we must setup the CLAM::Audio to hold the incoming samples from
	// each file channel.
	std::vector<CLAM::Audio> outputs;
	outputs.resize( reader.GetAudioFile().GetHeader().GetChannels() );

	// And now we set the size of each Audio object to our intended 'read size'
	for ( unsigned i = 0; i < outputs.size(); i++ )
	{
		outputs[i].SetSize( readSize );
	}

	// Now we can safely Start() the processing
	reader.Start();

	// We will implement a crappy text-based VUmeter just to some elementary
	// aspects of working with Audio's. What's a VUmeter? A vumeter is just
	// sort of gauge that indicates the loudness of a given signal. This
	// 'loudness' can be computed for each samples as:
	//     loudness[ n ] = 20. * log10( fabs( x[n] ) )
	// Note that this will give a number in the [0,-inf] range, in dB scale. What
	// is interesting is to see which is the range of this loudness for a given
	// audio fragment: it might be understood as a measure of how 'loud' is
	// that fragment.

	// We will use this counter for tracking the current frame index
	CLAM::TIndex frameCount = 1;

	// When will the loop end? Whenever the EOF is reached ;) i.e. when the Processing
	// cannot generate any more Audio objects.
	while ( reader.Do( outputs ) )
	{
		std::cout << "At frame #" << frameCount << std::endl;

		for ( unsigned int i = 0; i < outputs.size(); i++ )
		{
			std::cout << "Channel #" << i << " ";

			// We take the first sample loudness as both minimum and maximum
			CLAM::TData currentMax = 20. * log10( fabs(outputs[i].GetBuffer()[0]));
			CLAM::TData currentMin = 20. * log10( fabs(outputs[i].GetBuffer()[0]));
			
			// Simple min&max search loop: we compute loudness for each sample
			// in the fragment and check it against the current max and min
			for ( int n = 1; n < outputs[i].GetSize(); n++ )
			{
				CLAM::TData loudness = 20. * log10( fabs(outputs[i].GetBuffer()[n]) );
				if ( loudness >= currentMax )
					currentMax = loudness;
				if ( loudness <= currentMin )
					currentMin = loudness;
			}
			
			// we print out the pair of (max,min) loudness values
			std::cout << "( " << currentMax << " dB, " << currentMin << " dB)" << std::endl;
		}
		
		frameCount++;
	}

	// And stop it:
	reader.Stop();
	
	return 0;
}
