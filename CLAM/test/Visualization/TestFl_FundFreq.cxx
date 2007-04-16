#include <iostream>
#include "Segment.hxx"
#include "FundamentalAdapter.hxx"
#include "WidgetTKWrapper.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include "Fl_FundFreq.hxx"
#include <exception>
#include <string>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Window.H>

static std::string sFilename;


bool TestBasicUseCase( CLAMVM::FundamentalAdapter& adapter, CLAMVM::Fl_FundFreq& presentation )
{
	CLAM::XMLStorage x;
	CLAM::Segment    segment;
	
	x.Restore( segment, sFilename );

	std::cout << "SEGMENT RESTORED" << std::endl;

	adapter.BindTo( segment );
	
	adapter.Publish();
	

	Fl_Window* mainWindow = new Fl_Window( 100, 100, 640, 480, "Test" );

	mainWindow->add( &presentation );

	mainWindow->end();


	mainWindow->show();

	CLAMVM::WidgetTKWrapper& tk = CLAMVM::WidgetTKWrapper::GetWrapperFor("FLTK");

	tk.Run();

	return true;

}


int main ( int argc, char** argv )
{
	try
	{
		const char* str = fl_file_chooser( "Please, select analysis data", "*.xml", NULL );

		if ( !str )
		{
			std::cout << "No file was given!" << std::endl;
			exit(-1);
		}

		sFilename = str;

		CLAMVM::FundamentalAdapter adapter;
		CLAMVM::Fl_FundFreq        widget( 0,0,640, 480, "Test Fl_FundFreq presentation");

		adapter.TrajectoryExtracted.Connect( widget.NewTrajectory );
		adapter.TimeSpanChanged.Connect( widget.NewTimeSpan );

		std::cerr << "Basic Fl_FundFreq use case test launched" << std::endl;
		if ( !TestBasicUseCase( adapter, widget ) )
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

