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

// Welcome to the "FileInfo" example. In this example, we will see how we can
// check:
//   + That a given file is readable by CLAM
//   + Its header data
//   + Its 'text' descriptors

// This header imports CLAM::AudioFile class interface
#include "AudioFile.hxx"
#include <iostream>

// To make this example minimally interesting, we will accept one parameter to
// the program, which will be the filename we wish to inspect

int main( int argc, char** argv )
{

	if ( argc == 1 ) // No input file
	{
		std::cerr << "No input file" << std::endl;
		std::cerr << "Usage: FileInfo <input file name>" << std::endl;

		return -1;
	}
	else if ( argc > 2 ) // Too many parameters
	{
		std::cerr << "Too many parameters" << std::endl;
		std::cerr << "Usage: FileInfo <input file name>" << std::endl;
		
		return -1;
	}

	// Now we take argv[1] ( the second token in the command used to invoke this
	// program ) as the 'File Location'

	CLAM::AudioFileSource  file;
	file.OpenExisting( argv[1] );

	// Once set the location, the AudioFile object internally does several things:
	//   + It checks that the file exists and is readable ( it conforms to one of
	//     the supported formats )
	//   + Extracts useful header data, such as sampling rate, number of channels, etc.
	//   + Extracts some textual meta-data that could be embedded in the file itself

	// So first, let's check that the input file is readable
	if ( !file.IsReadable() )
	{
		std::cerr << "Error: file " << file.GetLocation() << " cannot be opened ";
		std::cerr << "or is encoded in an unrecognized format" << std::endl;
		return -1;
	}

	// If it was readable now we can safely assume we can access its header
	std::cout << "Filename: " << file.GetLocation() << std::endl;
	// Now we print out the format it was encoded:
	std::cout << "Format: " << file.GetHeader().GetFormat() << std::endl;
	// The number of channels in the file
	std::cout << "Channels: " << file.GetHeader().GetChannels() << std::endl;
	// Its sample rate
	std::cout << "Sample Rate: " << file.GetHeader().GetSampleRate() << " Hz" << std::endl;

	// And its length. However this is not so straight-forward since we provide
	// file duration in *milliseconds*. This might not be the best way ( we know ) but
	// for now, CLAM sees times as milliseconds. We can try to convert it into something like
	// hh:mm:ss

	CLAM::TTime durationMs = file.GetHeader().GetLength();
	
	
	CLAM::TTime numberSeconds = durationMs / 1000.;
	CLAM::TTime numberMinutes = (int)(numberSeconds / 60.);
	numberSeconds = numberSeconds - numberMinutes * 60.;
	CLAM::TTime numberHours = int(numberMinutes / 60.);
	numberMinutes = numberMinutes - numberHours * 60.;

	// and now we print it formatted:

	std::cout << "Length (hh:mm:ss): ";
	std::cout << numberHours << ":" << numberMinutes << ":" << numberSeconds << std::endl;

	// Finally, we can also access textual meta-data, if it was present in the file, of course.
	// AudioFile's textual meta-data is stored in an AudioTextDescriptors object, that we
	// can access as follows:

	const CLAM::AudioTextDescriptors& txtDesc = file.GetTextDescriptors();

	// For now, AudioTextDescriptors only can hold the following attributes:
	//  + Artist name
	//  + Title/Songname
	//  + Album/Movie/Show name
	//  + Track number ( in album )
	//  + Composer name
	//  + Performer name
	//
	//  Obviously, it will be strange that all (or any) of these attributes are available, so we must be
	// careful before accessing each attribute:
	if ( txtDesc.HasArtist() )
		std::cout << "Artist name: " << txtDesc.GetArtist() << std::endl;
	if ( txtDesc.HasTitle() )
		std::cout << "Title/Songname: " << txtDesc.GetTitle() << std::endl;
	if ( txtDesc.HasAlbum() )
		std::cout << "Album/Movie/Show name: " << txtDesc.GetAlbum() << std::endl;
	if ( txtDesc.HasTrackNumber() )
		std::cout << "Track number: " << txtDesc.GetTrackNumber() << std::endl;
	if ( txtDesc.HasComposer() )
		std::cout << "Composer name: " << txtDesc.GetComposer() << std::endl;
	if ( txtDesc.HasPerformer() )
		std::cout << "Performer name: " << txtDesc.GetPerformer() << std::endl;
	
	// And that's all friends!

	return 0;
}

