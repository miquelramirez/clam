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


/** The goals of this example are:
 *    -> load a .wav file into a CLAM::Audio object
 *    -> write a CLAM::Audio object into a .wav file
 */

#include <iostream> 
#include "AudioFileIn.hxx"       // this header imports the CLAM::AudioFileIn Processing class interface
#include "AudioFileOut.hxx"      // the same for the CLAM::AudioFileOut Processing class
#include "Audio.hxx"             // imports the CLAM::Audio ProcessingData class interface
#include "Assert.hxx"            // imports CLAM defensive programming utils
#include <FL/fl_file_chooser.H>  // imports FLTK functions for file choosing dialogs

int main( int argc, char** argv )
{

	CLAM::AudioFileIn   fileLoader; 
	CLAM::AudioFileConfig fileLoaderConfig; 

	// Before being able to use the AudioFileIn object we have just created, we must first 'configure'
	// it. This configuration involves both specifying the name of the .wav file we want to open, and
	// telling the sound file format expected ( such as WAVE RIFF ).

	// First, we obtain the filename from the user through a FLTK file chooser dialog. The function
	// will return either the filename selected by the user, or a NULL pointer, if the user 'cancels' the
	// dialog


	const char* filename = fl_file_chooser( "Please, select a .wav file", "*.wav", NULL );
	
	// the user might cancel the dialog, so we can default the filename variable value to some pre-established
	// value, or just abort program execution
	if ( filename == NULL )
	{
		std::cout << "User cancelled" << std::endl;
		exit(0);
	}

	// Once we have acquired the filename we set the appropiate values for the AudioFileInConfig object
	fileLoaderConfig.SetFilename( filename );

	// CLAM::EAudioFileType is an enum type that specifies CLAM Audio file I/O file formats supported
	fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave ); 

	// Now we attempt to configure the AudioFileIn object. Note that this may fail by a
	// number of reasons, such as an unexpected file format or a corrupted file.

	if ( !fileLoader.Configure( fileLoaderConfig ) )
	{
		// the attempt to configure the fileLoader object has failed
		// so we prompt an error message, and abort
		
		std::cerr << "ERROR: could not open " << filename << std::endl;
		exit(-1);
	}
	
	// Once the fileLoader object is configured we can ask him about two very important
	// characteristics of the audio we have loaded: its size in samples, and its sampling rate
	CLAM::TSize nSamples = fileLoader.Size();
	CLAM::TData sampleRate = fileLoader.SampleRate();
	
	// These parameters are necessary to setup the Audio object, which encapsulates the
	// sequence of samples that form the audio signal
	
	CLAM::Audio   signal;
	signal.SetSize( nSamples );
	signal.SetSampleRate( sampleRate );

	// Now we are prepared to extract the actual samples from the file and dump them into our
	// Audio object
	
	fileLoader.Start();      
	fileLoader.Do( signal );  // we tell the file loader to copy the actual samples into our Audio object
	fileLoader.Stop();

	// Once loaded into the Audio object, we could operate on the samples contained in it as we see fit. 
	// But here we will just dump back the Audio Object to disk

	CLAM::AudioFileOut  fileStorer;
	CLAM::AudioFileConfig fileStorerConfig;

	// Now, we just do the same steps as before...

	// Ask the user about the filename 

	const char* outputFilename = fl_file_chooser(  "Please, specify file to dump Audio object", "*.wav", NULL );
	
	// now, if the user cancels, it makes sense to specify a default filename

	if ( !outputFilename )
	{
		outputFilename = "output.wav";
		std::cout << "Audio samples will be stored on " << outputFilename << std::endl;
	}
	
	// now we set the values for the fileStorer object configuration

	fileStorerConfig.SetFilename( outputFilename );
	fileStorerConfig.SetFiletype( CLAM::EAudioFileType::eWave );

	// but we must, additionaly, specify now the sample rate we desire
	// the file to have
	fileStorerConfig.SetSampleRate( signal.GetSampleRate() );

	// We configure the fileStorer object
	fileStorer.Configure( fileStorerConfig );
	
	// Now we are prepared to dump the Audio object into disk

	fileStorer.Start();      
	fileStorer.Do( signal );  // we tell the file store to write the actual samples into the file
	fileStorer.Stop();
	
	// And that's all

	std::cout << "Have nice bash/tcsh!" << std::endl;

	return 0;
}
