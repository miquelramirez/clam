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
	
	char* dspFileToRead = 0;
	char dspFileToWrite[250];
	

	// TODO pass dsp file as option (no normal arg)
	if (argc!=3 && argc!=2)
	{
		fprintf(stderr,"Usage srcdeps SETTINGSFILE DSPFILE\n"
			"or, if you're generating a new dsp, srcdeps SETTINGSFILE\n" );
		exit(-1);
	}
	
	settingsFile = argv[1];
	if (argc==3) // dsp file is passed.
		dspFileToRead = argv[2];	

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
	printf("\t dspFileToRead: \t%s \n", dspFileToRead);
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

	if(dspFileToRead)
	{
//		if (files is equal to dspFileToWrite)
//			dsp_parse_inline(dspFileToWrite);
//		else
//			dsp_parse_from_file(dspFileToRead, dspFileToWrite);
		
		//provisionally, since is thw way Visual use it:
		dsp_parse_inplace(dspFileToRead);
	}
	else
		dsp_parse_from_empty(dspFileToWrite);

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
