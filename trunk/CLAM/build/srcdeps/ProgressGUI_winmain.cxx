#include "ProgressGUI.hxx"

#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#	include "parser.h"
#	include "dsp_parser.h"
#	include "config_parser.h"
}

void ProgressGUIAdapter::setUp( char **argv )
{
	gendepend = 0;
	recursesrcs = 1;
	
	config_init();
	
	listhash_add_item_str(config,"OS_WINDOWS","1");
	listhash_add_item_str(config,"OS_LINUX","0");

	config_parse(argv[1]);

	parser_init();

	config_check();
	
}

void ProgressGUIAdapter::finish( char **argv)
{

	dsp_parse(argv[2]);
	parser_exit();
	config_exit();

	FILE* f = fopen( "buildstamp", "w" );
	if ( !f ) 
	{
		fprintf( stderr, "Error: failed to create build stamp!" );
		exit( -1 );
	}
	fclose( f );

}


int main( int argc, char **argv)
{
	if (argc!=3)
	{
		fprintf(stderr,"Usage srcdeps SETTINGSFILE DSPFILE\n");
		exit(-1);
	}

	ProgressGUIAdapter adapter(argc,argv); // third ctr. argument is bool moreDetail=true
		
	return ProgressGUIAdapter::runEventsLoop();	
}
