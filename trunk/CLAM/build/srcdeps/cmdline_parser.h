#ifndef __cmdline_parser__
#define __cmdline_parser__

#include <stdio.h>
#include <stdlib.h>

struct Cmd_options
{
	char* settingsfile;
	char* srcfile;
	int verbose;
	int progress;
};

void print_cmd_usage()
{
	fprintf(stderr, "Usage: \n"
		"\n"
		"  srcdeps [-v] [-p] [-d SRCFILE] [SETTINGS.cfg]\n"
		"    Options goes in any order. Description:\n"
		"\n"
		"    v: verbose\n"
		"    p: show progress \n"
		"    d: generate dependences for SRCFILE. When this option is activated, \n"
		"       no Makefile.vars or MS Visual project file will be calculated.\n"
		"\n"
		"    If no settings file specified, it takes settings.cfg by default.\n"
		"\n"
		"    The 3 output files are: PROGNAME.dsp, PROGNAME.vcproj and Makefile.vars,\n"
		"    where PROGNAME is specifyed in the input settings file\n");
	exit(-1);
}

void init_cmd_options( struct Cmd_options* o )
{
	o->settingsfile = 0;
	o->srcfile = 0;
	o->verbose = 0;
	o->progress = 0;
}

void print_cmd_options( struct Cmd_options* o )
{
	fprintf(stderr, "settings = %s\nsrcfile = %s\nverbose = %s\nprogress = %s\n\n", 
		o->settingsfile,
		o->srcfile,
		o->verbose ? "true" : "false",
		o->progress ? "true" : "false");
}


// this function is the only one used by the main
int cmdline_parse(int argc, char* argv[], struct Cmd_options*  options)
{
	int i = 1;

	init_cmd_options(options);
	     
	// parse options:
	while ( i<argc )
	{
		if (argv[i][0] != '-') 
		{
			if (options->settingsfile)
			{
				fprintf(stderr, "Already have settingsfile '%s' specified when parsing argument '%s'\n",
				options->settingsfile,argv[i]);
				return 0;
			}
			options->settingsfile = argv[i++];
			continue;
		}
		// non parametrized options
		switch (argv[i][1])
		{
			case 'h':
				return 0;
			case 'v':
				options->verbose = 1;
				i++;
				break;
			case 'p':
				options->progress = 1;
				i++;
				break;
			case 'd':
				i++;
				if (argv[i][0] == '-') 
				{
					fprintf(stderr, "option -d must be followed by the SRCFILE argument\n");
					return 0;
				}
				options->srcfile = argv[i++];
				break;
			default:
				fprintf(stderr, "Invalid option %s\n",argv[i]);
				return 0;
		}
	}
	if (!options->settingsfile)
	{
		options->settingsfile = "settings.cfg";
	}
	return 1;
}


#endif
