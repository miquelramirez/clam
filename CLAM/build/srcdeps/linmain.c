#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "objdepname.h"
#include "config_parser.h"
#include "verbose.h"

#include "makegen.h"

int verbose = 0;

void usage(void)
{
	fprintf(stderr,"Usage srcdeps SETTINGSFILE\n");
	fprintf(stderr,"   or srcdeps -d SETTINGSFILE SRCFILE\n");
	exit(-1);
}


int main(int argc,char** argv)
{
	char* settingsfile = 0;
	char* srcfile = 0;
	int progress = 0;
	gendepend = 2;

	{
		int i;
		for (i=1;i<argc;i++)
		{
			if (argv[i][0]=='-')
				if (argv[i][1]=='d' && argv[i][2]==0)
					gendepend=1;
				else if (argv[i][1]=='p' && argv[i][2]==0)
					progress=1;
				else if (argv[i][1]=='v' && argv[i][2]==0)
					verbose=1;
				else
					usage();
			else
				if (settingsfile==0) settingsfile = argv[i];
				else if (srcfile==0) srcfile = argv[i];
				else
					usage();
		}
	}
	
	
	if (gendepend==1)
	{
		if (srcfile==0) usage();
	}
	else
	{
		if (settingsfile==0) usage();
	}
	
	config_init();

#ifdef __APPLE__
	listhash_add_item_str(config,"OS_WINDOWS","0");
	listhash_add_item_str(config,"OS_LINUX","0");
	listhash_add_item_str(config,"OS_MACOSX","1");
#else
	listhash_add_item_str(config,"OS_WINDOWS","0");
	listhash_add_item_str(config,"OS_LINUX","1");
	listhash_add_item_str(config,"OS_MACOSX","0");
#endif

	config_parse(settingsfile);

	if (gendepend==1)
	{
		/* important to the this before parser_init ! */
		listkey* k = listhash_find(config,"SOURCES");
		/* clear the list, we only want to process the 1 source file 
		** given on the command line
		*/
		list_clear(k->l);
		list_add_str(k->l,srcfile);
		recursesrcs = 0;
	}else{
		recursesrcs = 1;
	}
	
	parser_init();

	config_check();

	{
		int cnt = 0;
		item* i = guessed_sources->first;
		while (i)
		{
			if (progress)
			{
				fprintf(stderr,"%s %d %d\n",i->str,cnt,list_size(guessed_sources));
			}
		 parser_run(i->str);

		 i = i->next;
		 cnt++;
		}
	}

	if (gendepend==2)
	{
		makefilevars_generate();		
	}

	parser_exit();
	config_exit();

	return 0;
}
