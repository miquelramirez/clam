#include "ProgressGUI.hxx"

#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include "parser.h"
#include "includepaths.h"
#include "config_parser.h"
#include "objdepname.h"
#include "config_parser.h"
}

// begin functions defined in linmain
void usage(void)
{
	fprintf(stderr,"Usage srcdeps SETTINGSFILE\n");
	fprintf(stderr,"   or srcdeps -d SETTINGSFILE SRCFILE\n");
	exit(-1);
}

void makefilevars_generate(void)
{
	FILE* outfile = stdout;

	{
		listkey* k = listhash_find(config,"EXTRA_MAKEFILE_VARS");
		item* i = (k && k->l) ? k->l->first : 0;
		while (i)
		{	
			if (i->str && i->str[0]!=0)
			{
				fprintf(outfile,"%s = ",i->str);
				{
					listkey* kk = listhash_find(config,i->str);
					item* ii = (kk && kk->l) ? kk->l->first : 0;
					while (ii)
					{	
						if (ii->str && ii->str[0]!=0)
						{
							fprintf(outfile,"\\\n %s",ii->str);
						}
						ii = ii->next;
					}
				}
				fprintf(outfile,"\n\n");
			}
			i = i->next;
		}
	}

	{
		item* i = guessed_sources->first;
		fprintf(outfile,"SOURCES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
			{
				fprintf(outfile,"\\\n %s",i->str);
			}
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = guessed_sources->first;
		fprintf(outfile,"OBJECTS =");
		while (i)
		{
			char objname[2048];
			convert_to_objname(objname,2048,i->str);
			fprintf(outfile,"\\\n %s",objname);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = guessed_sources->first;
		fprintf(outfile,"DEPENDS =");
		while (i)
		{
			char depname[2048];
			if (i->str && i->str[0]!=0)
			{
				convert_to_depname(depname,2048,i->str);
				fprintf(outfile,"\\\n %s",depname);
			}
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = needed_includepaths->first;
		fprintf(outfile,"INCLUDES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -I%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"LIBRARY_PATHS");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"LIBRARY_PATHS =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -L%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"LIBRARIES_DEBUG");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"LIBRARIES_DEBUG =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -l%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"LIBRARIES_RELEASE");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"LIBRARIES_RELEASE =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -l%s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = defines->first;
		fprintf(outfile,"DEFINES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -D%s",i->str);
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		item* i = pre_includes->first;
		fprintf(outfile,"PRE_INCLUDES =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n -include %s",i->str);
			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"CXXFLAGS_DEBUG");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"CXXFLAGS_DEBUG =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n %s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"CXXFLAGS_RELEASE");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"CXXFLAGS_RELEASE =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n %s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"LINK_FLAGS_DEBUG");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"LINK_FLAGS_DEBUG =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n %s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	{
		listkey* k = listhash_find(config,"LINK_FLAGS_RELEASE");
		item* i = (k && k->l) ? k->l->first : 0;
		fprintf(outfile,"LINK_FLAGS_RELEASE =");
		while (i)
		{
			if (i->str && i->str[0]!=0)
				fprintf(outfile,"\\\n %s",i->str);

			i = i->next;
		}
		fprintf(outfile,"\n\n");
	}

	fclose(outfile);
}
// end functions defined in linmain


void ProgressGUIAdapter::setUp( char **argv )
{
	gendepend = 2;
	
	if (strcmp(argv[1],"-d")==0)
		gendepend=1;

	config_init();

	listhash_add_item_str(config,"OS_WINDOWS","0");
	listhash_add_item_str(config,"OS_LINUX","1");


	if (gendepend==1)
	{
		if (argc!=4) usage();
		config_parse(argv[2]);
	}
	else
	{
		if (argc!=2) usage();
		config_parse(argv[1]);
	}


	if (gendepend==1)
	{
		listkey* k = listhash_find(config,"SOURCES");
		list* sources = k->l;
		list_add_str(sources,argv[3]);
		recursesrcs = 0;
	}else{
		recursesrcs = 1;
	}
	
	parser_init();

	config_check();

}

void ProgressGUIAdapter::finish( char **argv)
{
	if (gendepend==2)
	{
		makefilevars_generate();		
	}

	parser_exit();
	config_exit();
}


int main( int argc, char **argv)
{
	if (argc<2) usage();
	

	ProgressGUIAdapter adapter(argc,argv); // third ctr. argument is bool moreDetail=true
		
	return ProgressGUIAdapter::runEventsLoop();	
}
