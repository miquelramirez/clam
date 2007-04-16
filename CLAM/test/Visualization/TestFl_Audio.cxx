#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "XMLStorage.hxx"
#include "FLTK/Fl_Audio.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>
#include <string>
#include "CLAM_Math.hxx"
#include "OSDefines.hxx"
#include "WidgetTKWrapper.hxx"
#include "AudioFileIn.hxx"
#include <FL/fl_file_chooser.H>

using CLAM::Err;
using CLAM::Audio;
using CLAM::XMLStorage;
using CLAM::AudioFileIn;
using CLAM::AudioFileConfig;
using CLAM::EAudioFileType;
using CLAMVM::AudioAdapter;
using CLAMVM::Fl_Audio;
using CLAMVM::WidgetTKWrapper;

static const char* sPathToData="./Datasets/";
static std::string sFilename;
static bool        sNoArgs;

void CreateDummyAudioObject( Audio& audioObj )
{
	const float srate = 44100;
	const float freq = 440;
	const float amp = 0.745f;
	const int  n = 4000;

	float deltaphi = (2 * M_PI * freq)/srate;

	// A = sin ( 2*pi*f*n/sr )

	audioObj.SetSampleRate( 8000 );
	audioObj.SetBeginTime( 0 );
	audioObj.SetEndTime( float(n)/srate );
	audioObj.SetSize( n );

	float phi = 0.0;

	for ( int i = 0; i < n; i++ )
	{
		audioObj.GetBuffer()[i] = amp * cos( phi );
		phi += deltaphi;
	}
}

bool LoadAudioObjectFromFile( Audio& obj ) throw ( Err )
{
		AudioFileIn fileLoader;
		AudioFileConfig loaderConfig;

		loaderConfig.SetFilename( sFilename );
//		loaderConfig.SetChannels( 2 );
		loaderConfig.SetFiletype( EAudioFileType::eWave );
		
		fileLoader.Configure( loaderConfig );

		fileLoader.Start();
	
		obj.SetSize( fileLoader.Size() );

		fileLoader.Do( obj );
		obj.SetSampleRate( fileLoader.SampleRate() );
		
		return true;
}

bool TestBasicUseCase( AudioAdapter& view, Fl_Audio& presentation ) throw ( Err )
{
		
		XMLStorage  x;
		Audio       audioObj;

		// TODO: Ask David

		//std::string filename = "Audio_2.xml";
		//std::string pathToFile = sPathToData;

		//pathToFile+=filename;

		//x.Restore( audioObj, pathToFile );

		if ( sNoArgs )
				CreateDummyAudioObject( audioObj );
		else
		{
				if (!LoadAudioObjectFromFile( audioObj ) )
						return false;
		}

		std::cout << "Dummy audio Object created" << std::endl;

		// View and Presentation now talk with each other

		view.BindTo( audioObj );

		view.Publish();
		presentation.Show();
		
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();

		return true;
}


int main( int argc, char** argv )
{
		AudioAdapter                view;
		Fl_Audio                    presentation(100,100,640,480, "An Audio Presentation");

		sNoArgs = false;

		const char* strfile = fl_file_chooser( "Please, select a .wav file", "*.wav", NULL );
		if ( strfile != NULL )
			sFilename = strfile;
		else // no args supplied
		{
				sNoArgs = true;
				std::cout << "No args provided, displayed audio will be generated" << std::endl;
		}
		
		presentation.AttachTo( view );

		try
		{
				if ( !TestBasicUseCase( view, presentation) )
						std::cerr << "Basic Use case Test...... FAILED!" << std::endl;
				else
						std::cerr << "Basic Use case Test...... Passed!" << std::endl;
				
		}
		catch ( CLAM::Err& e )
		{
				std::cerr << "A CLAM controlled error has occured" << std::endl;
				e.Print();
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( std::exception& e )
		{
				std::cerr << "An standard library error has occured" << std::endl;
				std::cerr << e.what() << std::endl;
				std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

				return -1;
		}
		catch( ... )
		{
				std::cerr << "An unknown error has occured!" << std::endl;
				return -1;
		}

		return 0;
}

