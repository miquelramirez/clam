#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"

#include "verbose.h"

int verbose = 0;

int main(int argc,char** argv)
{
	if (argc!=3)
	{
		fprintf(stderr,"Usage srcdeps SETTINGSFILE DSPFILE\n");
		exit(-1);
	}
	
	gendepend = 0;
	recursesrcs = 1;
	
	config_init();
	
	listhash_add_item_str(config,"OS_WINDOWS","1");
	listhash_add_item_str(config,"OS_LINUX","0");
	listhash_add_item_str(config,"OS_MACOSX","0");

	config_parse(argv[1]);

	parser_init();

	config_check();

	{
		int cnt = 0;
		item* i = guessed_sources->first;
		while (i)
		{
			fprintf(stderr,"%s %d %d\n",i->str,cnt,list_size(guessed_sources));

			parser_run(i->str);
			
			i = i->next;
			cnt++;
		}
	}

	dsp_parse(argv[2]);

	parser_exit();
	config_exit();
	
	{
		FILE* f = fopen( "buildstamp", "w" );
		if ( !f ) 
		{
			fprintf( stderr, "Error: failed to create build stamp!" );
			exit( -1 );
		}
		fclose( f );
	}
	return 0;
}
