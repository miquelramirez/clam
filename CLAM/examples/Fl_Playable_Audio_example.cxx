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

#include <iostream>
#include <string>

#include <FL/Fl.H>
#include <FL/fl_file_chooser.H>

#include "AudioPlayer.hxx"
#include "Fl_Browsable_Playable_Audio.hxx"

#include "AudioFileIn.hxx"
#include "Audio.hxx"
#include "AudioAdapter.hxx"

using namespace CLAM;

int main ()
{
	try {
		std::string fileName( fl_file_chooser("Choose file to load...", "*.wav", "") );

		std::cout << "Loading " << fileName.c_str() << " contents..." << std::endl;

		//The FileIn PO
		AudioFileConfig infilecfg;
		infilecfg.SetName( "FileIn" );
		infilecfg.SetFilename( fileName.c_str() );
		infilecfg.SetFiletype( EAudioFileType::eWave );

		AudioFileIn myAudioFileIn( infilecfg );

		// Initialization of the processing data objects :
		TSize fileSize=myAudioFileIn.Size();
		Audio myAudioIn;
		myAudioIn.SetSize(fileSize);

		myAudioIn.SetSampleRate( myAudioFileIn.SampleRate() );

		//Read Audio File
		myAudioFileIn.Start();
		myAudioFileIn.Do( myAudioIn );
		myAudioFileIn.Stop();

		std::cout << "Displaying " << fileName.c_str() << " contents..." << std::endl;

		CLAMVM::Fl_Browsable_Playable_Audio localPresentation( 0, 0, 600, 400, fileName.c_str() );
		CLAMVM::AudioAdapter soundView;

		soundView.BindTo( myAudioIn );
		localPresentation.AttachTo( soundView );
		
		soundView.Publish();
		localPresentation.Show();

		Fl::run();

		std::cout << "Exiting..." << std::endl;
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 1;
}
