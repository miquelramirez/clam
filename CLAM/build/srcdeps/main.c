#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline_parser.h"
#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "dsp_parser.h"
#include "vcproj_parser.h"
#include "makegen.h"
#include "verbose.h"

typedef enum 
{
	windows_platform, 
	linux_platform, 
	apple_platform
} Platform;

Platform platform;

int verbose = 0;

//Create moc-generated and uic-generated files folders
void createQtGenDirsIfNecessary()
{
	listkey* h = NULL;

	if (platform != windows_platform)
		return;

	h = listhash_find( config, "USE_QT" );
	if (!h) 
		return;
	
	if ( !strcmp(h->l->first->str,"1") )
	{
		fprintf( stderr, "Creating .\\moc folder...\n" );
		system( "mkdir moc" );
		fprintf( stderr, "Creating .\\uic folder...\n" );
		system( "mkdir uic" );
	}
}

void addQtGenDirsToIncludepaths()
{
	listkey* h = NULL;
	item* current = NULL;
	h = listhash_find( config, "UI_FILES" );
	if (!h) return;
	
	current = h->l->first;
	if ( current )
	{
		list_add_str_once( includepaths, "./uic" );
		list_add_str_once( needed_includepaths, "./uic");
	}

	if (verbose)
	{
		fprintf( stderr, "Files that need to be uic'ed\n" );
		while( current != NULL )
		{
			fprintf( stderr, "%s \n", current->str );
			current = current->next;
		}
	}
}



int main(int argc,char** argv)
{
	char dspFileToWrite[250];
	char vcprojFileToWrite[250];

	struct Cmd_options settings;
	if (!cmdline_parse(argc, argv, &settings ) )
	{
		print_cmd_usage( &settings );
		exit(-1);
	}
		
	//TODO remove:
	settings.progress = 1;

	print_cmd_options( &settings );
	verbose = settings.verbose;

	// TODO unify in one global var
	if (settings.srcfile)
	{
		gendepend = 1;
		recursesrcs = 0;
	}
	else
	{
		gendepend = 2;
		recursesrcs = 1;
	}
	
	config_init();

#ifdef __APPLE__
	platform = apple_platform;
	listhash_add_item_str(config,"OS_WINDOWS","0");
	listhash_add_item_str(config,"OS_LINUX","0");
	listhash_add_item_str(config,"OS_MACOSX","1");
#else
#	ifdef WIN32
	platform = windows_platform;
	listhash_add_item_str(config,"OS_WINDOWS","1");
	listhash_add_item_str(config,"OS_LINUX","0");
	listhash_add_item_str(config,"OS_MACOSX","0");
#	else // Linux
	platform = linux_platform;
	listhash_add_item_str(config,"OS_WINDOWS","0");
	listhash_add_item_str(config,"OS_LINUX","1");
	listhash_add_item_str(config,"OS_MACOSX","0");
#	endif
#endif

	config_parse( settings.settingsfile );
	sprintf(dspFileToWrite,"%s.dsp", program->first->str);
	sprintf(vcprojFileToWrite, "%s.vcproj", program->first->str);

	if (gendepend==1)
	{
		// important to do this before parser_init !
		listkey* k = listhash_find(config,"SOURCES");
		// clear list, we only want to process the src file given on the cmd line
		list_clear(k->l);
		list_add_str(k->l, settings.srcfile);
		fprintf(stderr, "in gendepend==1 : added file: %s recurse: %i\n", settings.srcfile, recursesrcs);
	}

	parser_init();
	config_check();

	// .cxx list loop
	{
		int cnt = 0;
		item* i = guessed_sources->first;
		if (settings.progress) fprintf(stderr, "Searching for needed source files:\n");
		while (i)
		{
			if (settings.progress)
			{
				fprintf(stderr,"%d of %d done\t%s\n",cnt,list_size(guessed_sources), i->str);
				fflush(stderr);
			}

			parser_run(i->str);

			i = i->next;
			cnt++;
		}
	}

	createQtGenDirsIfNecessary();
	
	addQtGenDirsToIncludepaths();


	if (platform != windows_platform && gendepend==2)
	{
		makefilevars_generate();
	}

	if (platform == windows_platform )
	{
		dsp_parse( dspFileToWrite );
		vcproj_parse( vcprojFileToWrite);
	}

	parser_exit();
	config_exit();
	
	return 0;
}
