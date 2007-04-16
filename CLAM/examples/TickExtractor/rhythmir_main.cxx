/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "Err.hxx"
#include "rhythmir_dumper.hxx"
#include "rhythmir_cmdproc.hxx"
#include "rhythmir_process.hxx"
#include "Pool.hxx"
#include <exception>
#include <string>
#include <cstdlib>
#include "Meter.hxx"

int main( int argc, char** argv )
{

	try
	{
		RhythmIR::CmdLineParms parms = RhythmIR::CmdLineParms::ParseArgs( argc, argv );
		
		CLAM::Pulse tickData;
		CLAM::Pulse beatData;
		
		CLAM::DescriptionScheme scheme;
		
		scheme.AddAttribute<std::string>              ( "Global", "Units.Onset.Position" );
		scheme.AddAttribute<std::string>              ( "Global", "Units.Beat.Position");
		scheme.AddAttribute<std::string>              ( "Global", "Units.Tick.Position");
		scheme.AddAttribute<std::string>              ( "Global", "Path" );
		scheme.AddAttribute<CLAM::TData>              ( "Global", "SampleRate" );
		scheme.AddAttribute<unsigned>                 ( "Global", "BeatsPerMinute" );
		scheme.AddAttribute<unsigned>                 ( "Global", "TicksPerMinute" );
		scheme.AddAttribute<CLAM::RhythmDescription::Meter> ( "Global", "Meter" );


		scheme.AddAttribute<CLAM::TData>              ( "Sample", "Value" );
		scheme.AddAttribute<CLAM::TData>              ( "Sample", "NormalizedValue" );

		scheme.AddAttribute<CLAM::TTime>              ( "Onset",  "Position" );
		scheme.AddAttribute<CLAM::TData>              ( "Onset",  "Weight" );

		scheme.AddAttribute<CLAM::TTime>              ( "Beat",   "Position");
		scheme.AddAttribute<CLAM::TTime>              ( "Tick",   "Position");
		
		CLAM::DescriptionDataPool pool( scheme );

		pool.SetNumberOfContexts( "Global", 1 );

		*pool.GetWritePool<std::string>("Global", "Units.Onset.Position" ) = "SampleIndex";
		*pool.GetWritePool<std::string>("Global", "Units.Beat.Position" ) = "Seconds";
		*pool.GetWritePool<std::string>("Global", "Units.Tick.Position" ) = "Seconds";
		*pool.GetWritePool<std::string>("Global", "Path" ) = parms.GetInputFilename();


		/* Description extraction*/		  		  

		RhythmIR::LoadInputAudio( pool, parms.GetInputFilename() );

		RhythmIR::NormalizeInputAudio( pool, parms.GetConfig() );

		RhythmIR::ExtractOnsets( pool, parms.GetConfig() );

		RhythmIR::ExtractTicksAndBeats( pool, parms.GetConfig() );

		RhythmIR::ExtractMeter( pool, parms.GetConfig() );
		
		/* Description storage*/

		std::cout << "Dumping data on " << parms.GetOutputFilename() << "..." << std::endl;
	
		RhythmIR::DumpToSimacXML( pool, parms.GetOutputFilename() );		
		RhythmIR::DumpToWavesurferLabs( pool, parms.GetOutputFilename() );

		/*
		CLAM::ExtractTicksSequence( parms.GetInputFilename(), 
					    parms.GetConfig(), 
					    tickData, beatData );
		
		DumpExtractedData( parms.GetOutputFilename(), parms.GetInputFilename(), 
				   tickData, beatData );
		*/
		
	}
	catch( CLAM::Err& e )
	{
		std::cerr << "ERROR: CLAM exception caught at top level: " << e.what() << std::endl;
	}
	catch( std::exception& e )
	{
		std::cerr << "ERROR: standard exception caught: " << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cerr << "ERROR: Unknown exception caught at top level" << std::endl;
		exit( -1 );
	}


	return 0;
}

