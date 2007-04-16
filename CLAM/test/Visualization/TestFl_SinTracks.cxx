#include "Segment.hxx"
#include "SinTracksAdapter.hxx"
#include "Fl_SinTracks.hxx"
#include "WidgetTKWrapper.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>
#include <string>
#include <FL/fl_file_chooser.H>

using CLAM::Segment;
using CLAM::XMLStorage;
using CLAMVM::SinTracksAdapter;
using CLAMVM::Fl_SinTracks;
using CLAMVM::WidgetTKWrapper;

static const char* sPathToData = "./Datasets/";
static const char* sFilename = 0;

bool TestBasicUseCase( SinTracksAdapter& view, Fl_SinTracks& presentation )
{
	XMLStorage x;
	Segment    segmentObj;
		
/*		std::string filename = "Segment.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;
*/
	x.Restore( segmentObj, sFilename );

	std::cout << "SEGMENT RESTORED" << std::endl;

	view.BindTo( segmentObj );

	view.Publish();
	std::cout << "DATA ACQUISITION DONE!" << std::endl;
	presentation.Show();

	WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

	tk.Run();

		

	return true;
}

int main( int argc, char** argv )
{
	try
	{
		sFilename = fl_file_chooser( "Please, select analysis data", "*.xml", NULL );

		if ( sFilename == NULL )
		{
			std::cout << "Quitting!" << std::endl;
			exit(0);
		}

		SinTracksAdapter             view;
		Fl_SinTracks   presentation(100,100,640,480, "Some sinusoidal tracks...");

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

