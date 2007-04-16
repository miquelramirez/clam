#include "AudioPlot.hxx"
#include "SpectrumPlot.hxx"
#include "SinTracksPlot.hxx"
#include "SpectrumAndPeaksPlot.hxx"
#include "SystemPlots.hxx"
#include "FundFreqPlot.hxx"
#include "Segment.hxx"
#include "XMLStorage.hxx"
#include "AudioFileIn.hxx"       // this header imports the CLAM::AudioFileIn Processing class interface
#include <string>
#include "Err.hxx"
#include <exception>
#include <iostream>
#include <FL/fl_file_chooser.H>

void loadWAVEFile( const std::string& filename,
		   CLAM::Audio& object )
{
	CLAM::AudioFileIn   fileLoader; 
	CLAM::AudioFileConfig fileLoaderConfig; 

	fileLoaderConfig.SetFilename( filename );

	fileLoaderConfig.SetFiletype( CLAM::EAudioFileType::eWave ); 

	if ( !fileLoader.Configure( fileLoaderConfig ) )
	{
		std::cerr << "ERROR: could not open " << filename << std::endl;
		exit(-1);
	}
	
	CLAM::TSize nSamples = fileLoader.Size();
	CLAM::TData sampleRate = fileLoader.SampleRate();
		
	object.SetSize( nSamples );
	object.SetSampleRate( sampleRate );

	fileLoader.Start();      
	fileLoader.Do( object );
	fileLoader.Stop();

}

void loadSegment( const std::string& filename,
		  CLAM::Segment& object )
{
	CLAM::XMLStorage x;

	x.Restore( object, filename );
}

int main( int argc, char** argv )
{
	try
	{
		// Data objects
		CLAM::Segment                theData;
		CLAM::Audio                  theWAVEFile;

		// Plots
		CLAMVM::AudioPlot            theAudioPlot;
		theAudioPlot.SetLabel( "An audio signal" );
		theAudioPlot.SetSize( 640, 480 );
		CLAMVM::SpectrumPlot         theSpectrumPlot;
		theSpectrumPlot.SetLabel( "An spectrum" );
		theSpectrumPlot.SetSize( 640, 480 );
		CLAMVM::SpectrumAndPeaksPlot theSpecAndPeaksPlot;
		theSpecAndPeaksPlot.SetLabel( "An spectrum and detected peaks" );
		theSpecAndPeaksPlot.SetSize( 640, 480 );
		CLAMVM::SinTracksPlot        theSinTracksPlot;
		theSinTracksPlot.SetLabel( "Sinusoidal tracks in a segment" );
		theSinTracksPlot.SetSize( 640, 480 );
		CLAMVM::FundFreqPlot         theFundFreqPlot;
		theFundFreqPlot.SetLabel( "Fundamental frequency in a segment" );
		theFundFreqPlot.SetSize( 640, 480 );

		// filenames
		std::string  selectedWaveFile;
		std::string  selectedXMLFile;

		const char* tmp = NULL;

		tmp = fl_file_chooser( "Select the WAVE file to load", "*.wav", NULL, 1 );
		
		if ( tmp )
			selectedWaveFile = tmp;

		tmp = fl_file_chooser( "Select the SMS Tools XML analysis file to load", "*.xml", NULL, 1 );

		if ( tmp )
			selectedXMLFile = tmp;
	       
		std::cout << "Loading WAVE file... " ;
		std::cout.flush();
		loadWAVEFile( selectedWaveFile, theWAVEFile );
		std::cout << "DONE!" << std::endl;
		std::cout << "Loading Segment... ";
		std::cout.flush();
		loadSegment( selectedXMLFile, theData );
		std::cout << "DONE!" << std::endl;
		// plots data setting

		theAudioPlot.SetData( theWAVEFile );
		
		CLAM::Spectrum& anSpectrumInTheMiddle = 
			theData.GetFrame( theData.GetnFrames() / 2 ).GetResidualSpec();
		theSpectrumPlot.SetData( anSpectrumInTheMiddle );

		CLAM::Spectrum& anotherSpectrumInTheMiddle =
			theData.GetFrame( theData.GetnFrames() / 2).GetSinusoidalAnalSpectrum();
		CLAM::SpectralPeakArray& attachedPeakArray =
			theData.GetFrame( theData.GetnFrames() / 2).GetSpectralPeakArray();

		theSpecAndPeaksPlot.SetData( anotherSpectrumInTheMiddle, attachedPeakArray );
		

		theSinTracksPlot.SetData( theData );
		theFundFreqPlot.SetData( theData );

		CLAMVM::SystemPlots::DisplayAll();

	}
	catch( CLAM::Err& e )
	{
		std::cerr << "A CLAM error occurred!" << std::endl;
		e.Print();
		exit(-1);
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		exit( -1 );
	}
	catch( ... )
	{
		std::cerr << "Unknown exception caught" << std::endl;
		exit( -1 );
	}

	return 0;
}

