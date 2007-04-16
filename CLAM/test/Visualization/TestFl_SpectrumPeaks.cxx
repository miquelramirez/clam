#include "Frame.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "SpectralPeakArrayAdapter.hxx"
#include "Fl_SpectrumPeaks.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include "WidgetTKWrapper.hxx"
#include <iostream>
#include <string>
#include <exception>

using CLAM::SpectralPeakArray;
using CLAM::Spectrum;
using CLAM::Frame;
using CLAM::XMLStorage;
using CLAMVM::LogMagSpectrumAdapter;
using CLAMVM::SpectralPeakArrayAdapter;
using CLAMVM::Fl_SpectrumPeaks;
using CLAMVM::WidgetTKWrapper;

static const char* sPathToData="./Datasets/";

bool TestBasicUseCase( LogMagSpectrumAdapter& specView, SpectralPeakArrayAdapter& peaksView, Fl_SpectrumPeaks& peaksWidget )
{
		XMLStorage  x;
		Spectrum spec;
		SpectralPeakArray peaks;
		
		std::string filename1 = "ResidualSpectrum.xml";
		std::string pathToFile1 = sPathToData;
		
		pathToFile1+=filename1;

		std::string filename2 = "SpectralPeakArray.xml";
		std::string pathToFile2 = sPathToData;

		pathToFile2+=filename2;

		x.Restore( peaks, pathToFile2 );

		x.Restore( spec, pathToFile1 ); // The frame has been incorporated into the system

		specView.BindTo( spec );
		peaksView.BindTo( peaks );

		specView.Publish();
		peaksView.Publish();

		peaksWidget.Show();

		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		tk.Run();
		
		return true;
}

int main( int argc, char** argv )
{
		try
		{
				LogMagSpectrumAdapter                   spectrumView;
				SpectralPeakArrayAdapter                peaksView;
				Fl_SpectrumPeaks                        presentation( 100, 100, 640, 480, "Spectrum + peaks" );

				presentation.AttachTo( spectrumView, peaksView );
				
				std::cerr << "BASIC SPECTRAL PEAK ARRAY USE CASE TEST LAUNCHED" << std::endl;
				if ( !TestBasicUseCase( spectrumView, peaksView, presentation ) )
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

