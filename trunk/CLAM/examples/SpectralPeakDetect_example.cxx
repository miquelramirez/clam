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


#include "SpectralPeakDetectExampleHelper.hxx"
#include "SpectralPeakDetect.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralAnalysisExampleHelper.hxx"
#include "XMLStorage.hxx"
#include "Err.hxx"
#include <iostream>
#include <FL/fl_file_chooser.H>


int main ( int argc, char** argv )
{
	try
	{

		CLAMExamples::SpectralAnalysis restoredAnalysis;
		
		const char* analysisName = fl_file_chooser( "Please select an Spectral Analysis", "*.xml", NULL );
		
		if ( analysisName == NULL )
		{
			std::cout << "User cancelled!" << std::endl;
			exit( 0 );
		}
		
		CLAM::XMLStorage storageIFace;

		storageIFace.Restore( restoredAnalysis, analysisName );

		CLAM::SpectralPeakDetectConfig processingConfig;
		
		processingConfig.SetMaxPeaks( 10 );
		processingConfig.SetMagThreshold( -80 );
		processingConfig.SetMaxFreq( restoredAnalysis.GetAnalysisSettings().GetSampleRate()/4 );
		
		CLAM::SpectralPeakDetect peakDetector;

		peakDetector.Configure( processingConfig );
		
		peakDetector.Start();

		CLAMExamples::SpectralPeaksSet detectedPeaksSet;

		detectedPeaksSet.SetSpectralAnalysis( analysisName );
		detectedPeaksSet.SetPeakDetectionSettings( processingConfig );

		CLAMExamples::SpectralAnalysis::FramesContainer::iterator i =
			restoredAnalysis.GetResultingFrames().begin();

		CLAM::SpectralPeakArray tmpPeakArray;
		tmpPeakArray.SetScale( CLAM::EScale::eLog );

		while( i!= restoredAnalysis.GetResultingFrames().end() )
		{
			i->ToDB();

			peakDetector.Do( *i, tmpPeakArray );

			detectedPeaksSet.GetDetectedPeaks().push_back( tmpPeakArray );

			i++;
		}

		peakDetector.Stop();

		storageIFace.Dump( detectedPeaksSet, "DetectedPeaks_0001", "DetectedPeaks_0001.xml" );
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
