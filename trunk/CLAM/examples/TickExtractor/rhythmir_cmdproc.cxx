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

#include "XMLStorage.hxx"
#include "rhythmir_cmdproc.hxx"
#include <getopt.h>
#include <cstdlib>

namespace RhythmIR
{
	static const struct option longCmdLineOpts[] =
	{
		{"help", 0, NULL, 'h'},
		{"input", 1, NULL, 'i'},
		{"output", 1, NULL, 'o'},
		{"config", 1, NULL, 'c'},
		{NULL, 0, NULL, 0}
	};

	static const char* shortCmdLineOpts = "hi:o:c:";

	void CmdLineParms::PrintHelp()
	{
		std::cout << "rhythmir is a CLAM example that analyzes input audio and extract significative" << std::endl;
		std::cout << "rhythm information: the set of ticks, beats. Also calculates their rates (beats per minute)/(ticks per minute)."<< std::endl;
		std::cout << "The following parameters are accepted:" << std::endl;
		std::cout << "-h/--help              Prints this help" << std::endl;
		std::cout << "-i/--input  <filename> Allows to set the file to be analyzed. Compressed ( ogg/vorbis, mpeg) " << std::endl;
		std::cout << "                       files are accepted. [Mandatory]"<< std::endl;
		std::cout << "-o/--output <filename> Allows to set the name of the file where extracted information will be" << std::endl;
		std::cout << "                       dumped into. [Mandatory]" << std::endl;
		std::cout << "-c/--config <filename> Specifies the file where configuration for the information extraction " << std::endl;
		std::cout << "                       algorithms will be obtained. [Optional]" << std::endl;
		std::cout << std::endl;
		std::cout << "Example:" << std::endl;
		std::cout << "\t\t$ rhythmir -c myConfig.xml -i foo.wav -o foo_rythm_description.xml" << std::endl;
		std::cout << std::endl;
		std::cout << "foo.wav file will be opened and analyzed taking into account the parameters specified in " << std::endl;
		std::cout << "myConfig.xml. The extracted description will be stored on the 'foo_rhythm_description.xml'" << std::endl;
	}

	CmdLineParms::CmdLineParms( std::string in, std::string out, const CLAM::TickExtractorConfig& cfg )
		: mInputFilename( in ),
		  mOutputFilename( out ),
		  mConfig( cfg )
	{
	}

	CmdLineParms CmdLineParms::ParseArgs( int argc, char** argv )
	{
		int nextCommand;
		std::string inputFilename;
		std::string outputFilename;
		CLAM::TickExtractorConfig cfg;
	
		do
		{
			nextCommand = getopt_long( argc, argv, shortCmdLineOpts, longCmdLineOpts, NULL );

			switch ( nextCommand )
			{
			case 'h':
				PrintHelp();
				exit(0);
				break;
			case '?':
				PrintHelp();
				exit(-1);
				break;
			case 'i':
				inputFilename = optarg;
				std::cerr << "Analyzing " << inputFilename << "..." << std::endl;
				break;
			case 'o':
				outputFilename = optarg;
				break;
			case 'c':
				std::cout << "Loading config from " << optarg << std::endl;
				CLAM::XMLStorage::Restore( cfg, optarg );
				break;
			case -1:
				break;
			default:
				std::cerr << "ERROR: No parameters specified!" << std::endl;
				PrintHelp();
				exit(-1);
			};

		} while ( nextCommand != -1 );

		if ( inputFilename.empty() ) 
		{
			std::cerr << "ERROR: No input file was specified!" << std::endl;
			PrintHelp();
			exit(-1);
		}

		if ( outputFilename.empty() )
		{
			outputFilename = inputFilename;
			outputFilename += ".rythm_description.xml";
		}
		
		return CmdLineParms( inputFilename, outputFilename, cfg );
	}
	
}

