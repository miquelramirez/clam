#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "objdepname.h"
#include "parser.h"
#include "config_parser.h"
#include "makegen.h"

/* Private vars */
static list* gen_ui_objs = NULL;

/* Private module functions declaration*/

static void generate_uic_dependencies( FILE* outfile );

static void generate_moc_dependencies( FILE* outfile );

static void generate_extra_makefile_vars( FILE* outfile );

static void generate_sources_vars( FILE* outfile );

static void generate_objects_var( FILE* outfile );

static void generate_depends_var( FILE* outfile );

static void generate_includes_var( FILE* outfile );

static void generate_library_paths_var( FILE* outfile );

static void generate_libraries_debug_var( FILE* outfile );

static void generate_libraries_release_var( FILE* outfile );

static void generate_defines_var( FILE* outfile );

static void generate_pre_includes_var( FILE* outfile );

static void generate_cxxflags_debug_var( FILE* outfile );

static void generate_cxxflags_release_var( FILE* outfile );

static void generate_link_flags_debug_var( FILE* outfile );

static void generate_link_flags_release_var( FILE* outfile );


/* Private module functions implementation */

static void generate_uic_dependencies( FILE* outfile )
{
	listkey* currentUI = NULL;
	
	if ( ui_outputs->first == NULL )
		return;
	else
		currentUI = ui_outputs->first;
	
	
	
	while( currentUI != NULL )
	{
		char hdrwopath[2048];
		char objname[2048];
		char* hdrname = currentUI->l->first->str;
		char* srcname = currentUI->l->first->next->str;
		
		fprintf( stderr, "Generating rules for %s ...\n", currentUI->str );

		convert_to_objname( objname, 2048, srcname );
		discard_path( hdrwopath, 2048, hdrname );


		list_add_str_once( mocable_headers, hdrname );
		list_add_str_once( gen_ui_objs, objname );

		/* dependency writing */
		fprintf( outfile, "%s : %s\n", hdrname, currentUI->str );
		fprintf( outfile, "%s : %s\n", srcname, currentUI->str );
		fprintf( outfile, "%s : %s %s\n", objname, srcname, hdrname );
		fprintf( outfile, "\n" );
		
		currentUI = currentUI->next;
	}
	
}

static void generate_moc_dependencies( FILE* outfile )
{
	list* moc_objects;
	item* current;
	assert( mocable_headers != NULL );
	
	moc_objects = list_new();
	current = mocable_headers->first;
		
	while( current != NULL )
	{
		char mocname[2048];
		char objname[2048];
		
		fprintf( stderr, "Mocable header found: %s \n", current->str );
		convert_to_mocname( mocname, 2048, current->str );
		convert_to_objname( objname, 2048, mocname );
		list_add_str_once( moc_objects, objname );
		/*dependency printing */
		fprintf( outfile, "%s : %s %s\n", objname, mocname, current->str );
		fprintf( outfile, "%s : %s \n", mocname, current->str );
		fprintf( outfile, "\n" );
		current = current->next;
	}

	/*MOC_OBJECTS var*/
	current = moc_objects->first;
	fprintf( outfile, "MOC_OBJECTS = ");
	while( current!=NULL )
	{
		fprintf( outfile, "\\\n %s", current->str );
		current = current->next;
	}
	fprintf( outfile, "\n\n" );
	
	list_free( moc_objects );
	
}

static void generate_extra_makefile_vars( FILE* outfile )
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

static void generate_sources_vars( FILE* outfile )
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

static void generate_objects_var( FILE* outfile )
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

	i = gen_ui_objs->first;

	while( i != NULL )
	{
		fprintf( outfile, "\\\n %s", i->str );
		
		i = i->next;
	}

	fprintf(outfile,"\n\n");
}

static void generate_depends_var( FILE* outfile )
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

static void generate_includes_var( FILE* outfile )
{
	item* i = includepaths->first;
	fprintf(outfile,"INCLUDES =");
	while (i)
	{
		if (i->str && i->str[0]!=0)
			if (list_find(needed_includepaths,i->str))
				fprintf(outfile,"\\\n -I%s",i->str);
		
		i = i->next;
	}
	fprintf(outfile,"\n\n");
}

static void generate_library_paths_var( FILE* outfile )
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

static void generate_libraries_debug_var( FILE* outfile )
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

static void generate_libraries_release_var( FILE* outfile )
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

static void generate_defines_var( FILE* outfile )
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

static void generate_pre_includes_var( FILE* outfile )
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

static void generate_cxxflags_debug_var( FILE* outfile )
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

static void generate_cxxflags_release_var( FILE* outfile )
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

static void generate_link_flags_debug_var( FILE* outfile )
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

static void generate_link_flags_release_var( FILE* outfile )
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



/* Public module function implementation */

void makefilevars_generate(void)
{
	FILE* outfile = stdout;

	gen_ui_objs = list_new();

	generate_uic_dependencies( outfile );

	generate_moc_dependencies( outfile );

	generate_extra_makefile_vars( outfile );

	generate_sources_vars( outfile );

	generate_objects_var( outfile );

	generate_depends_var( outfile );

	generate_includes_var( outfile );

	generate_library_paths_var( outfile );

	generate_libraries_debug_var( outfile );

	generate_libraries_release_var( outfile );

	generate_defines_var( outfile );

	generate_pre_includes_var( outfile );

	generate_cxxflags_debug_var( outfile );

	generate_cxxflags_release_var( outfile );

	generate_link_flags_debug_var( outfile );

	generate_link_flags_release_var( outfile );

	list_free( gen_ui_objs );

	fclose(outfile);
}
