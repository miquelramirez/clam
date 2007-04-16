#include "Audio.hxx"
#include "AudioAdapter.hxx"
#include "XMLStorage.hxx"
#include "StdioAudioPresentation.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>

using CLAM::Audio;
using CLAM::XMLStorage;
using CLAMVM::AudioAdapter;
using CLAMVM::StdioAudioPresentation;

static const char* sPathToData="./Datasets/";

bool TestBasicUseCase( AudioAdapter& view, StdioAudioPresentation& presentation )
{
		
	XMLStorage  x;
	Audio       audioObj;

	std::string filename = "Audio.xml";
	std::string pathToFile = sPathToData;

	pathToFile+=filename;

	x.Restore( audioObj, pathToFile );

	// View and Presentation now talk with each other

	view.BindTo( audioObj );

	view.Publish();
	presentation.Show();
	
	return true;
}


int main( int argc, char** argv )
{
	AudioAdapter                view;
	StdioAudioPresentation      presentation;

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

