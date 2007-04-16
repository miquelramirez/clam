#include "Segment.hxx"
#include "SinTracksAdapter.hxx"
#include "StdioSinTracksPresentation.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>
#include <string>

using CLAM::Segment;
using CLAM::XMLStorage;
using CLAMVM::SinTracksAdapter;
using CLAMVM::StdioSinTracksPresentation;

static const char* sPathToData = "./Datasets/";

bool TestBasicUseCase( SinTracksAdapter& view, StdioSinTracksPresentation& presentation )
{
		XMLStorage x;
		Segment    segmentObj;
		
		std::string filename = "Segment.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;

		x.Restore( segmentObj, pathToFile );

		std::cout << "SEGMENT RESTORED" << std::endl;

		view.BindTo( segmentObj );

		view.Publish();
		std::cout << "DATA ACQUISITION DONE!" << std::endl;
		presentation.Show();

		return true;
}

int main( int argc, char** argv )
{
		try
		{
				SinTracksAdapter             view;
				StdioSinTracksPresentation   presentation;

				presentation.AttachTo( view );

				std::cerr << "BASIC SPECTRAL PEAK ARRAY USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestBasicUseCase( view, presentation ) )
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

