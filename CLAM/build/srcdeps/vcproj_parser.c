#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "config_parser.h"
#include "list.h"
#include "strfuncs.h"
#include "listhash.h"
#include "objdepname.h"

extern char* empty_vcproj_lines[];

static FILE* outfile = 0;

/* Main functions for mapping folder structure into virtual vc6 folders
 * structure. The filetype parameter indicates the kind of files we are
 * going to insert: 0 for regular c/c++ source files (.c, .C, .cpp, .cxx, etc. ),
 * 1 for c/c++ headers ( .h, .hxx, etc. ) and 2 for Qt ui files.
 * Now it this parameter is an enum (also conserving old int values)
 */

static void vcproj_parse_insert( FileType filetype );
static void vcproj_parse_insert_recurse( tree* t, list* repeatCheck, FileType type, int depth );

static void vcproj_parse_insert_regular_file( const char*, FileType type, int depth );
static void vcproj_parse_insert_ui_file( const char* file, int depth);
static void vcproj_parse_insert_mocable_header( const char* file, int depth);


static void indent( int numTabs );

void vcproj_parse_add_needed_includepaths(void)
{
	item* i = includepaths->first;
	int first = 1;
	while (i)
	{
		// we actually are traversing "needed_includepaths" but we do it this
		// strange way because we want to preserve the order as it is in "includepaths"
		if (i->str && i->str[0]!=0 && list_find(needed_includepaths,i->str) )
		{
			char tmp[1024];
			
			if (!first) stradd(",");
			else first = 0;
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			stradd(tmp);
		}
		i = i->next;
	}
}

// private function used from release and debug versions
void vcproj_parse_add_libraries(int isDebug)
{
	int first = 1;
	item* i;
	if (isDebug)
		i = vc7_libraries_debug->first;
	else
		i = vc7_libraries_release->first;

	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			if (!first) stradd(" ");
			else first = 0;
			stradd(i->str);
			stradd(".lib");
		}
		i = i->next;
	}
}
void vcproj_parse_add_defines(void)
{
	int first=1;
	item* i = defines->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			if (!first) stradd(";");
			else first = 0;
			stradd(tmp);
		}
		i = i->next;
	}
}
	
void vcproj_parse_add_release_libraries(void)
{
	vcproj_parse_add_libraries(0/* is NOT debug*/);
}

void vcproj_parse_add_debug_libraries(void)
{
	vcproj_parse_add_libraries(1/* IS debug*/);
}

void vcproj_parse_add_library_paths(void)
{
	int first = 1;
	item* i = library_paths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			winstyle(tmp);

			if (!first) stradd(",");
			else first = 0;
			stradd(tmp);
		}
		i = i->next;
	}
}

char* srcdeps_path()
{
	char* toppath = listhash_find(config, "TOP")->l->first->str;
	static char srcdepspath[100];
	sprintf(srcdepspath,"%s%s",toppath,"/build/srcdeps/srcdeps.exe");
	winstyle(srcdepspath);
	return srcdepspath;
}

/** The public function to be called from the main */
extern void vcproj_parse(const char* outFilename)
{
	// Parser based (conceptually) in a state machine
	typedef enum { visualStudioProject, configRelease, configDebug, files, theRest } ParserStates;
	ParserStates state;
	int nline = 0;

	outfile = fopen(outFilename, "w");
	state = visualStudioProject;
	while(empty_vcproj_lines[nline])
	{
		const char* line = empty_vcproj_lines[nline]; 
		if(state==visualStudioProject)
		{
			if ( strstr(line, "\tName=") )
			{
				fprintf(outfile, line, program->first->str );
			}
			else
			{
				fprintf(outfile, line);
				if ( strstr(line, "<Platforms>") )
					state = configRelease;
			}
		}
		else if(state==configRelease)
		{
			if (strstr(line, "\tAdditionalIncludeDirectories="))
			{
				char includesbuf[4096];
				strstart(includesbuf, 4096);
				vcproj_parse_add_needed_includepaths( );
				fprintf(outfile, line, includesbuf);
				strend();
			}
			else if ( strstr(line, "\tPreprocessorDefinitions=") )
			{
				char definesbuf[4096];
				strstart(definesbuf, 4096);
				vcproj_parse_add_defines();
				fprintf(outfile, line, definesbuf);
				strend();
			}
			else if(strstr(line,"\tForcedIncludeFiles=" ) )
			{ 
				fprintf(outfile, line, pre_includes->first->str);
			}
			else if ( strstr(line,"\tAdditionalDependencies=") )
			{
				char libsbuf[4096];
				strstart(libsbuf, 4096);
				vcproj_parse_add_release_libraries();
				fprintf(outfile, line, libsbuf);
				strend();
			}
			else if ( strstr(line, "\tOutputFile=") )
			{
				fprintf( outfile, line, program->first->str );
			}
			else if ( strstr(line, "\tAdditionalLibraryDirectories=") )
			{
				char libdirsbuf[4096];
				strstart(libdirsbuf, 4096);
				vcproj_parse_add_library_paths();
				fprintf( outfile, line, libdirsbuf );
				strend();
			}
			else  // tag not found. check if we must change state
			{
				fprintf(outfile, line);
				if ( strstr(line, "Name=\"Debug|Win32\"") )
						state = configDebug;
			}
		}
		else if(state == configDebug)
		{
			if (strstr(line, "\tAdditionalIncludeDirectories="))
			{
				char includesbuf[4096];
				strstart(includesbuf, 4096);
				vcproj_parse_add_needed_includepaths( );
				fprintf(outfile, line, includesbuf);
				strend();
			}
			else if ( strstr(line, "\tPreprocessorDefinitions=") )
			{
				char definesbuf[4096];
				strstart(definesbuf, 4096);
				vcproj_parse_add_defines();
				fprintf(outfile, line, definesbuf);
				strend();
			}
			else if(strstr(line,"\tForcedIncludeFiles=" ) )
			{ 
				fprintf(outfile, line, pre_includes->first->str);
			}
			else if ( strstr(line,"\tAdditionalDependencies=") )
			{
				char libsbuf[4096];
				strstart(libsbuf, 4096);
				vcproj_parse_add_debug_libraries();
				fprintf(outfile, line, libsbuf);
				strend();
			}
			else if ( strstr(line, "\tOutputFile=") )
			{
				fprintf( outfile, line, program->first->str );
			}
			else if ( strstr(line, "\tAdditionalLibraryDirectories=") )
			{
				char libdirsbuf[4096];
				strstart(libdirsbuf, 4096);
				vcproj_parse_add_library_paths();
				fprintf( outfile, line, libdirsbuf );
				strend();
			}
			else
			{
				fprintf(outfile, line);
				if ( strstr(line, "<Files>"))						
					state = files;
			}	
		}
		else if(state == files)
		{

			vcproj_parse_insert( source );
			vcproj_parse_insert( header );
			vcproj_parse_insert( qt );
			
			fprintf(outfile, line);
			state = theRest;

		}
		else // state == theRest
		{
			if ( strstr(line, "\tCommandLine=") )
			{
					fprintf( outfile, line, srcdeps_path() );
			}
			else
			{
				fprintf(outfile, line);
			}
		}
		nline++;
	}
	fclose(outfile);
	{
		FILE* stamp = fopen("buildstamp", "w");
		fclose(stamp);
	}
}


void vcproj_parse_insert(FileType type)
{
	char* typestr = NULL;
	list* filelist;
	tree* filetree = tree_new();
	list* repeatcheck = list_new();

	if ( type == header ) 
	{
		typestr = "Header Files";
		filelist = guessed_headers;
	}
	else if ( type == source ) 
	{ 
		typestr = "Source Files";
		filelist = guessed_sources;
	}
	else if ( type == qt ) 
	{
		typestr = "Qt ui Files";
		filelist = ui_files;
	}
	else
	{
		typestr = "Unknown files :o";
		filelist = NULL;
	}


	generate_files_tree(filelist, filetree);

	indent(2);
	fprintf(outfile,"<Filter Name=\"%s\" Filter=\"\">\n", typestr);
	vcproj_parse_insert_recurse(filetree,repeatcheck,type,3);
	indent(2);
	fprintf(outfile,"</Filter>\n");

	list_free(repeatcheck);
	tree_free(filetree);
}

void indent( int numTabs )
{
	int i;	
	for(i=0;i<numTabs;i++)
		fprintf(outfile, "\t");
}

void vcproj_parse_insert_recurse(tree* t,list* repeatcheck, FileType type, int depth)
{
	node * n = t->first;
	const char* typestr = filetype_str(type);
	while (n)
	{
		if (n->sub)
		{
			/* stupid visual cannot deal with same name at different levels */
			int cnt = 0;
			item* i = repeatcheck->first;
			while (i)
			{
				if (!strcmp(i->str,n->str)) cnt++;
				i = i->next;
			}
			list_add_str(repeatcheck,n->str);
			
			indent(depth);
			if (cnt>0)
			{
				fprintf(outfile,"<Filter Name=\"%s %s No. %d\" Filter=\"\">\n",n->str,typestr,cnt);
			}else{
				fprintf(outfile,"<Filter Name=\"%s %s\" Filter=\"\">\n",n->str,typestr);
			}
			//recursive call
			vcproj_parse_insert_recurse(n->sub,repeatcheck,type, depth+1 );
			indent(depth);
			fprintf(outfile,"</Filter>\n");
		}else{ // parsing node content (files in a subdir)
			if ( type == header )
			{
				assert( mocable_headers != NULL );
				
				if ( list_find( mocable_headers, n->str ) )
					vcproj_parse_insert_mocable_header( n->str, depth );
				else
					vcproj_parse_insert_regular_file( n->str, type, depth );
			}
			else if ( type == qt )
			{
				vcproj_parse_insert_ui_file( n->str, depth );
			}
			else
				vcproj_parse_insert_regular_file( n->str, type, depth );
		}
		n = n->next;
	}
}


void vcproj_parse_insert_regular_file( const char* filename, FileType type, int depth) 
{
	static char tmp[1024];
	strncpy(tmp,filename,1024);
	winstyle(tmp);
	indent(depth);
	fprintf(outfile, "<File RelativePath=\"%s\">\n", tmp);
	if (type==source)
	{
		indent(depth+1);
 		fprintf(outfile, "<FileConfiguration Name=\"Release|Win32\">\n");
		indent(depth+2);
		fprintf(outfile, "<Tool Name=\"VCCLCompilerTool\" />\n");
		indent(depth+1);
		fprintf(outfile, "</FileConfiguration>\n");
		indent(depth+1);
		fprintf(outfile, "<FileConfiguration Name=\"Debug|Win32\">\n");
		indent(depth+2);
		fprintf(outfile, "<Tool Name=\"VCCLCompilerTool\" />\n");
		indent(depth+1);
		fprintf(outfile, "</FileConfiguration>\n");
	}
	indent(depth);
	fprintf(outfile, "</File>\n");
}

static void vcproj_parse_insert_ui_file( const char* file, int depth)
{
	char winfile[1024];
	/*
	$(QTDIR)\bin\uic.exe $(InputPath) -o .\uic\$(InputName).h
	$(QTDIR)\bin\uic.exe $(InputPath) -i .\uic\${InputName}.h -o .\uic\$(InputName).cxx
	$(QTDIR)\bin\moc.exe -i .\uic\$(InputName).h -o .\moc\$(InputName)_moc.cxx
	*/

	strncpy(winfile,file,1024);
	winstyle(winfile);
	indent(depth);
	fprintf(outfile, "<File RelativePath=\"%s\">\n", winfile);
	indent(depth+1);
	fprintf(outfile, "<FileConfiguration Name=\"Debug|Win32\">\n");
	indent(depth+2);
	fprintf(outfile, "<Tool\n");
	indent(depth+3);
	fprintf(outfile, "Name=\"VCCustomBuildTool\"\n");
	indent(depth+3);
	fprintf(outfile, "Description=\"UICing %s\"\n", winfile);
	indent(depth+3);
	fprintf(outfile, "CommandLine=\"$(QTDIR)\\bin\\uic.exe $(InputPath) -o .\\uic\\$(InputName).h\n");
	indent(depth+4);
	fprintf(outfile, "$(QTDIR)\\bin\\uic.exe $(InputPath) -i $(InputName).h -o .\\uic\\$(InputName).cxx\n");
	indent(depth+4);
	fprintf(outfile, "$(QTDIR)\\bin\\moc.exe .\\uic\\$(InputName).h -o .\\moc\\$(InputName)_moc.cxx\"\n");
	indent(depth+3);
	fprintf(outfile, "Outputs=\".\\uic\\$(InputName).h;.\\uic\\$(InputName).cxx;.\\moc\\$(InputName)_moc.cxx\" />\n");
	indent(depth+1);
	fprintf(outfile, "</FileConfiguration>\n");
	indent(depth);
	fprintf(outfile, "</File>\n");

}

void vcproj_parse_insert_mocable_header( const char* file, int depth) 
{
	char winfile[1024];

	strncpy(winfile,file,1024);
	winstyle(winfile);
	indent(depth);
	fprintf(outfile, "<File RelativePath=\"%s\">\n", winfile);
	indent(depth+1);
	fprintf(outfile, "<FileConfiguration Name=\"Debug|Win32\">\n");
	indent(depth+2);
	fprintf(outfile, "<Tool\n");
	indent(depth+3);
	fprintf(outfile, "Name=\"VCCustomBuildTool\"\n");
	indent(depth+3);
	fprintf(outfile, "Description=\"MOCing %s\"\n", winfile);
	indent(depth+3);
	fprintf(outfile, "CommandLine=\"$(QTDIR)\\bin\\moc.exe $(InputPath) -o .\\moc\\$(InputName)_moc.cxx\"\n");
	fprintf(outfile, "Outputs=\".\\moc\\$(InputName)_moc.cxx\" />\n");
	indent(depth+1);
	fprintf(outfile, "</FileConfiguration>\n");
	indent(depth);
	fprintf(outfile, "</File>\n");

}



