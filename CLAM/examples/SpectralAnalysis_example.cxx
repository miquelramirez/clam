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

#include "AudioFileIn.hxx"
#include "SpectralAnalysisExampleHelper.hxx"
#include "XMLStorage.hxx"
#include "Plot.hxx"
#include "SystemPlots.hxx"
#include <FL/fl_file_chooser.H>
#include <iostream>

/**
 *  WARNING: This example should be considered as *WORK IN PROGRESS*
 */

int main( int argc, char** argv )
{
	try
	{
		CLAMExamples::SpectralAnalysisSettings settings;
		CLAM::XMLStorage storageIface;
		
		storageIface.Dump( settings, "AnSpectralAnalysisSettigns", "example_Config.xml" );

		const char* filename = fl_file_chooser( "Please select the Spectral Analysis settings to be used",
							"*.xml", NULL );

		if ( filename == NULL )
		{
			std::cerr << "User cancelled" << std::endl;
			exit(0);
		}
		


		storageIface.Restore( settings, filename );
		
		std::cout << "Specified config loaded" << std::endl;

		// Let's first spectral analysis settings

		CLAM::AudioFileIn     fileLoader;
		CLAM::AudioFileConfig fileLoaderConfig;

		filename = fl_file_chooser( "Please select the .wav file with the signal to be analyzed" , 
					    "*.wav", NULL );

		if ( filename == NULL )
		{
			std::cerr << "user cancelled" << std::endl;
			exit(0);
		}

		fileLoaderConfig.SetFilename( filename );
		fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave );
		fileLoader.PreConcreteConfigure( fileLoaderConfig );

		bool everythingOK = false;

		while ( !everythingOK )
		{
			try
			{
				fileLoader.ConcreteConfigure( fileLoaderConfig );
				everythingOK = true;
			}
			catch( CLAM::ErrSoundFileIO& error )
			{
				std::string messageShown = "Sorry, but I could not open the file \n";
				messageShown += fileLoaderConfig.GetFilename();
				messageShown += "\n";
				messageShown += "due to the following problem: \n";
				messageShown +=error.what();
				fl_message( messageShown.c_str() );
				const char* filename = fl_file_chooser( "Please, select a .wav file", "*.wav", NULL );
				if ( filename == NULL )
				{
					std::cout << "User cancelled" << std::endl;
					exit(0);
				}

				fileLoaderConfig.SetFilename( filename );				
			}
		}

		fileLoader.PostConcreteConfigure();


		settings.SetSampleRate( fileLoader.SampleRate() );

		CLAMExamples::SpectralAnalysis resultingAnalysis;
		resultingAnalysis.SetAnalyzedAudio( filename );

		CLAMExamples::SpectralFourierAnalyzer analyzer;

		analyzer.Configure( settings );

		CLAM::Audio inputBuffer;

		inputBuffer.SetSize( 256 );
		inputBuffer.SetSampleRate( fileLoader.SampleRate() );

		(*fileLoader.GetOutPorts().Begin())->Attach( inputBuffer );
		(*analyzer.GetInPorts().Begin())->Attach( inputBuffer );
		(*analyzer.GetOutPorts().Begin())->Attach( resultingAnalysis );

		fileLoader.Start();
		analyzer.Start();

		fileLoader.Do();

		do {
			analyzer.Do();
			std::cerr << "+";
			std::cerr.flush();
		} while( fileLoader.Do() );

		std::cerr << std::endl;

		fileLoader.Stop();
		analyzer.Stop();

		std::string defaultFilename( "SpectralAnalysis.xml" );

		filename = fl_file_chooser( "Please enter the filename where the analysis is to be stored",
					    "*.xml", NULL );
		if (  filename )
		{
			storageIface.Dump( resultingAnalysis, "SpectralAnalysis", filename );
		}
		else
			storageIface.Dump( resultingAnalysis, "SpectralAnalysis", "SpectralAnalysis_0001.xml");
	}
	catch( CLAM::Err& error )
	{
		std::cerr << "A CLAM exception was thrown:" << std::endl;
		std::cerr << error.what() << std::endl;
		exit( -1 );
	}
	catch( std::exception& error )
	{
		std::cerr << "An Standard Library exception was thrown: " << std::endl;
		std::cerr << error.what() << std::endl;
		exit( -1 );
	}
	catch( ... )
	{
		std::cerr << "An unexpected exception was thrown!" << std::endl;
		exit( -1 );
	}

	return 0;
}
