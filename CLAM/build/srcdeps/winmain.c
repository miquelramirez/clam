#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"

#include "verbose.h"

int verbose = 0;

/*
str_copy_path_from_file(char* destbuf, char* file, int max)
{
	int posOfLastSlash = -1;
	int currentPos = 0;
	char* current = file;
	while( *current )
	{
		if (*current=='/' || *current=='\\')
		{
			posOfLastSlash=currentPos;
		}
		++currentPos;
		++current;
	}
	current = file;

	for (currentPos=0; currentPos<=posOfLastSlash; currentPos++)
	{
		*destbuf = *current;
		++destbuf;
		++current;
	}
	*destbuf = 0; // termination character

	if ( posOfLastSlash >= max-1 )
	{
		fprintf(stderr,"cannot copy path of file: %s (too long for the passed buffer)", file);
		exit(-1);
	}
}
*/

int main(int argc,char** argv)
{
	char* settingsFile;	
	char dspFileToWrite[250];
	

	// TODO pass dsp file as option (no normal arg)
	if ( argc < 2 )
	{
		fprintf(stderr,"ERROR: Missing parameters!\n""Usage: srcdeps.exe <settings file> \n" );
		exit(-1);

	}
	
	settingsFile = argv[1];

	// by default we'll use the emptydsp which is preloaded as a C variable


	gendepend = 0;
	recursesrcs = 1;

	config_init();

	listhash_add_item_str(config,"OS_WINDOWS","1");
	listhash_add_item_str(config,"OS_LINUX","0");
	listhash_add_item_str(config,"OS_MACOSX","0");

	config_parse(settingsFile);

	
	// TODO: decide can settings file have path ?
	//str_copy_path_from_file(dspFileToWrite, settingsFile, 250);
	sprintf(dspFileToWrite,"%s.dsp", program->first->str);
	
	printf("\n\t settingsFile: \t%s \n", settingsFile);
	printf("\t dspFileToWrite: \t%s \n", dspFileToWrite);

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

	/*Create moc-generated and uic-generated files folders*/	
	{
		listkey* h = NULL;
		if ( ( h = listhash_find( config, "USE_QT" ) ) != NULL )
		{
			if ( !strcmp(h->l->first->str,"1") )
			{
				fprintf( stderr, "Creating .\\moc folder...\n" );
				system( "mkdir moc" );
				fprintf( stderr, "Creating .\\uic folder...\n" );
				system( "mkdir uic" );
			}
						
		}

		h = listhash_find( config, "UI_FILES" );

		if ( h )
		{
			item* current = h->l->first;
			fprintf( stderr, "Files that need to be uic'ed\n" );

			if ( current )
			{
				list_add_str_once( includepaths, "./uic" );
				list_add_str_once( needed_includepaths, "./uic");
			}
			
			while( current != NULL )
			{
				fprintf( stderr, "%s \n", current->str );
				current = current->next;
			}
		}
	}
	
	dsp_parse( dspFileToWrite );


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
