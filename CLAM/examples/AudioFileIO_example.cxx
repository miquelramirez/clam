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
#include "FileChooser.hxx"       // imports CLAMVM generic file chooser dialog
#include "MessageBox.hxx"        // generic message box

int main( int argc, char** argv )
{

	CLAM::AudioFileIn   fileLoader; 
	CLAM::AudioFileConfig fileLoaderConfig; 

	// Before being able to use the AudioFileIn object we have just created, we must first 'configure'
	// it. This configuration involves both specifying the name of the .wav file we want to open, and
	// telling the sound file format expected ( such as WAVE RIFF ).

	// First, we obtain the filename from the user through a CLAMVM::FileChooserDialog, a generic file chooser
	// widget dialog

	CLAMVM::FileChooserDialog fileDlg;
	
	// We set the dialog title
	fileDlg.SetTitle( "Please, select a .wav file" );
	// We add a file type to be filtered in by the dialog
	fileDlg.AddFileType( "*.wav" );
	fileDlg.AddFileType( "*.aiff" );

	// the user might cancel the dialog, so we can default the filename variable value to some pre-established
	// value, or just abort program execution
	if ( !fileDlg.Show() )
	{
		std::cout << "User cancelled" << std::endl;
		exit(0);
	}

	// Once we have acquired the filename we set the appropiate values for the AudioFileInConfig object
	fileLoaderConfig.SetFilename( fileDlg.GetSelectedFilename() );

	// CLAM::EAudioFileType is an enum type that specifies CLAM Audio file I/O file formats supported
	fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave ); 

	// When we Configure() or ConcreteConfigure() the AudioFileIn object it checks that the file specified in the
	// configuration is ( in order ):
	// 1) a valid file i.e. a readable, existing file
	// 2) it is a valid WAVE/RIFF file
	// 3) its encoding is supported by CLAM
	// Since this may fail due to a user mishap ( or that CLAM does not support the
	// encoding found on that file), we must be able to react to this condition by
	// first, catching possible exceptions and giving the user the possibility to
	// retry.
	// The difference between Configure() and ConcreteConfigure() is that the later will
	// throw exceptions with type so client applications can 'decorate' or handle different
	// error conditions as they see fit for their purpose. Note also that when using
	// ConcreteConfigure() instead of Configure() you must first call PreConcreteConfigure()
	// then make the call to ConcreteConfigure and finally call PostConcreteConfigure()

	fileLoader.PreConcreteConfigure( fileLoaderConfig );
	bool everythingOK = false;

	while ( !everythingOK )
	{
		try
		{
			fileLoader.ConcreteConfigure( fileLoaderConfig );

			// if there are no problems everythingOK will be true
			// and program will continue
			everythingOK = true;
		}
		catch( CLAM::ErrSoundFileIO& error )
		{
			// We will build an 'error string' that conveys useful information
			// for the user:
			std::string messageShown = "Sorry, but I could not open the file \n";
			messageShown += fileLoaderConfig.GetFilename();
			messageShown += "\n";
			messageShown += "due to the following problem: \n";
			messageShown +=error.what();
			
			CLAMVM::MessageBox msgBox;

			msgBox.SetText( messageShown );
			msgBox.Show();

			// And now we tell the user to provide a new filename, just
			// as we did before						
			
			if ( !fileDlg.Show() )
			{
				std::cout << "User cancelled" << std::endl;
				exit(0);
			}
				
			fileLoaderConfig.SetFilename( fileDlg.GetSelectedFilename() );
		}
	}
	
	fileLoader.PostConcreteConfigure();
	
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
	
	// We start the processing: this implies that the file will be opened
	// for reading audio data from the file
	fileLoader.Start(); 
	fileLoader.Do( signal );  // we tell the file loader to copy the actual samples into our Audio object
	// Now the file will be closed
	fileLoader.Stop();

	// Once loaded into the Audio object, we could operate on the samples contained in it as we see fit. 
	// But here we will just dump back the Audio Object to disk

	CLAM::AudioFileOut  fileStorer;
	CLAM::AudioFileConfig fileStorerConfig;

	// Now, we just do the same steps as before...

	// Ask the user about the filename 

	fileDlg.SetTitle("Please, specify file to dump Audio object" );
	std::string outputFilename;

	// now, if the user cancels, it makes sense to specify a default filename

	if ( !fileDlg.Show() )
	{
		outputFilename = "output.wav";
		std::cout << "Audio samples will be stored on " << outputFilename << std::endl;
	}
	
	outputFilename = fileDlg.GetSelectedFilename();

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
