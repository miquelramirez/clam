#include "SpectralPeakArray.hxx"
#include "SpectralPeakArrayAdapter.hxx"
#include "StdioSpectralPeakArrayPresentation.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include <iostream>
#include <exception>
#include <string>

using CLAM::SpectralPeakArray;
using CLAM::XMLStorage;
using CLAMVM::SpectralPeakArrayAdapter;
using CLAMVM::StdioSpectralPeakArrayPresentation;

static const char* sPathToData = "./Datasets/";

bool TestBasicUseCase( SpectralPeakArrayAdapter& view, StdioSpectralPeakArrayPresentation& presentation )
{
		XMLStorage x;
		SpectralPeakArray    specPeakArrayObj;
		
		std::string filename = "SpectralPeakArray.xml";
		std::string pathToFile = sPathToData;

		pathToFile+=filename;

		x.Restore( specPeakArrayObj, pathToFile );

		view.BindTo( specPeakArrayObj );

		view.Publish();
		presentation.Show();

		return true;
}

int main( int argc, char** argv )
{
		try
		{
				SpectralPeakArrayAdapter                view;
				StdioSpectralPeakArrayPresentation   presentation;

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

