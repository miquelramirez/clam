#include <iostream>
#include <string>

#include "SMSRealtimeSynth.hxx"

void Usage( char* programName )
{
	            std::cerr << "Usage: " << programName << " <sms configuration> <sms analysis file> <sms transformation score>" << std::endl;
}

void CheckArgs( int argc, char* argv[] )
{
	if ( argc == 4 ) return;

	if ( argc == 1 ) 
	{
	       		std::cerr << "No configuration file" << std::endl;
				Usage( argv[0] );
	            exit ( -1 );
	 }

	if ( argc == 2 ) 
	{
	       		std::cerr << "No analysis file" << std::endl;
				Usage( argv[0] );
	            exit ( -1 );
	 }

	if ( argc == 3 ) 
	{
	       		std::cerr << "No transformation score" << std::endl;
				Usage( argv[0] );
	            exit ( -1 );
	}

	if ( argc > 4 ) 
			{
	            std::cerr << "Too many parameters" << std::endl;
				Usage( argv[0] );
        	    exit ( -1 );
	        }
}

int main( int argc, char* argv[] )
{
	CheckArgs( argc, argv );

	CLAM::AudioManager audioManager( 44100, 512 );

	std::cout << "\n >>> SMS Realtime Synth <<<\n\n";
	std::cout << "Sample rate: " << "44100\n\n";
	std::cout << "Configuration file: " << argv[1] << std::endl;
	std::cout << "Analysis file: " << argv[2] << std::endl;
	std::cout << "Transformation score: " << argv[3] << std::endl << std::endl;

	CLAM::SMSRealtimeSynth synth( 44100 );

	synth.Run();

	synth.QueryState();

	std::cout << "Before LoadConfiguration...\n";
	synth.LoadConfigurationFile( argv[1] );
	
	std::cout << "Before LoadAnalysis...\n";
	synth.LoadAnalysisFile( argv[2] );

	std::cout << "Before LoadScoreFile...\n";
	synth.LoadScoreFile( argv[3] );
	
	synth.QueryState();

//	std::cout << "And now the transfom:\n";  	

//	synth.Play();
	synth.Stream();

	return 0;
}

