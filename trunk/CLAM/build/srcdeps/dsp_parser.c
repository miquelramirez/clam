#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "config_parser.h"
#include "listhash.h"
#include "tree.h"
#include "stack.h"
#include "list.h"
#include "strfuncs.h"
#include "objdepname.h"

stack* groupstack = 0;

FILE* infile = 0;
FILE* outfile = 0;

extern char* empty_dsp_lines[];

static int skip = 0;
static char* endgroup = "# End Group";
static char* begingroup = "# Begin Group \"";

static int sourcefile = 0; 
/* 1 = inside Begin..End Source File, before SOURCE=
** 2 = inside Begin..End Source File, after SOURCE= 
*/

char currentConfig[256];
int  currentConfigIsDebug = 0;

static int skipsource = 0;
static int begingroupl = 0;

/* Private utility functions */


/* Optimization over C standard library own strcmp based
 * on the assumption that comparing equal strings will be
 * the most frequent case.
 * MRJ: Is this absolutely necessary? It is not 
 * implemented elsewhere? Why does it call strcmp internally
 * when the strings are not equal?
 */
static int strcmp_begin( const char*, const char* b );

/* Overload of strcmp that ignores differences between
 * strings if that difference consists in a different
 * new-line writing i.e. Unix LF vs. Windows CR+LF
 */
static int strcmp_eol( const char* a, const char* b );

static void assert_file_open( FILE* fd, const char* filename, const char* errDescription );

/* Dsp structures generators */

/*Private functions for generating project settings */
static void dsp_parse_add_needed_includepaths( void );
static void dsp_parse_add_pre_includes( void );
static void dsp_parse_add_defines( void );
static void dsp_parse_add_cxxflags( void );
static void dsp_parse_add_libraries( void );
static void dsp_parse_add_library_paths( void );
static void dsp_parse_add_link_flags( void );

/* Project "source" files insertion  routines*/


/* Main functions for mapping folder structure into virtual vc6 folders
 * structure. The filetype parameter indicates the kind of files we are
 * going to insert: 0 for regular c/c++ source files (.c, .C, .cpp, .cxx, etc. ),
 * 1 for c/c++ headers ( .h, .hxx, etc. ) and 2 for Qt ui files.
 * Now it this parameter is an enum (also conserving old int values)
 */

static void dsp_parse_insert( FileType filetype );
static void dsp_parse_insert_recurse( tree* t, list* repeatCheck, FileType type );

/* Shorthand functions for calling dsp_parse_insert() function
 * passing the adequate value for type
 */
static void dsp_parse_insert_sources();
static void dsp_parse_insert_headers();
static void dsp_parse_insert_ui_rules();

/* Private function for adding the settings.cfg build rule for updating
 * the .dsp when necessary
 */
static void dsp_parse_insert_settings_rule(void);

/* Inserts a file without custom build rules associated*/
static void dsp_parse_insert_regular_file( const char* filename );

/* Private functions for inserting into .dsp file appropiated rules
 * for handling headers that require the moc precompiling step.
 */
static void dsp_parse_insert_mocable_header( const char* mocableFile );
static void dsp_parse_insert_moc_custom_build_rule( const char* fileString  );

/* Private functions for inserting into .dsp appropiated rules
 * for handling Qt's .ui files.
 */
static void dsp_parse_insert_ui_rules();
static void dsp_parse_insert_ui_file( const char* uiFile );
static void dsp_parse_insert_ui_custom_build_rule( const char* uiFile );

/*Main .dsp generation routine*/
static void dsp_parse_line( const char* buf, int line );

/* Checks syntax of damned ! IF !ELSEIF construct for discriminating
 * between Release and Debug builds
 */
static void dsp_parse_line_chkcfg(const char* buf,int line);

/* Public module functions implementation */

void dsp_parse(const char* outFilename)
{

	char buf[4096];
	int line = 0;

	groupstack = stack_new();

	outfile = fopen(outFilename,"w");
	assert_file_open(outfile, outFilename, "for writing");
	
	while (empty_dsp_lines[line])
	{
		strncpy(buf, empty_dsp_lines[line], 4096);
		line++;
		dsp_parse_line(buf,line);
	}
	fclose(outfile);
}

/* Private module functions implementation */

int strcmp_begin(const char* a,const char* b)
{
	while (*a && *b && *a==*b)
	{
		a++; b++;
	}
	if (*b==0) return 0;
	
	return strcmp(a,b);
}

int strcmp_eol(const char* a,const char* b)
{
	while (*a && *b && *a==*b)
	{
		a++; b++;
	}
	if (
		(*a==0 || *a=='\r' || *a=='\n') &&
		(*b==0 || *b=='\r' || *b=='\n') ) return 0;
	
	return strcmp(a,b);
}

/* helper function. Maybe should be in some other place? (PA) */
void assert_file_open(FILE* fd, const char* filename, const char* additional)
{
	if (!fd)
	{
		fprintf(stderr, "Could not open %s %s\n", filename, additional);
		exit(-1);
	}
}


void dsp_parse_add_needed_includepaths(void)
{
	item* i = includepaths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{	
			if (list_find(needed_includepaths,i->str))
			{
				char tmp[1024];
				strncpy(tmp,i->str,1024);
				winstyle(tmp);
				stradd(" /I \"");
				stradd(tmp);
				stradd("\"" );
			}
		}
		i = i->next;
	}
}

void dsp_parse_add_pre_includes(void)
{
	item* i = pre_includes->first;
	/* visual is stupid. /FI accepts a filename only, not a path,
	** so we have to split this in /I and /FI
	while (i)
	{
		char tmp[1024];
		strncpy(tmp,i->str,1024);
		winstyle(tmp);
		stradd(" /FI\"");
		stradd(tmp);
		stradd("\"" );
		i = i->next;
	}
	*/
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			char* ptr;
			char* filename = tmp;
			strncpy(tmp,i->str,1024);
			winstyle(tmp);
			ptr = tmp;
			while (*ptr)
			{
				if (*ptr=='\\') filename = ptr;
				ptr++;
			}
			if (*filename=='\\')
			{
				*filename = 0;
				filename++;
				stradd(" /I \"");
				stradd(tmp);
				stradd("\"" );
			}
			stradd(" /FI\"");
			stradd(filename);
			stradd("\"" );
		}
		i = i->next;
	}
}

void dsp_parse_add_defines(void)
{
	item* i = defines->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			stradd(" /D \"");
			stradd(tmp);
			stradd("\"" );
		}
		i = i->next;
	}
}

void dsp_parse_add_cxxflags(void)
{
	item* i = currentConfigIsDebug ?
		cxxflags_debug->first 
		:
		cxxflags_release->first 
	;
	while (i) 
	{
		if (i->str && i->str[0]!=0)
		{
			stradd(" ");
			stradd(i->str);
		}
		i = i->next;
	}
}

void dsp_parse_add_libraries(void)
{
	item* i = currentConfigIsDebug ?
		libraries_debug->first 
		:
		libraries_release->first 
	;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			stradd(" ");
			stradd(i->str);
			stradd(".lib");
		}
		i = i->next;
	}
}

void dsp_parse_add_library_paths(void)
{
	item* i = library_paths->first;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			char tmp[1024];
			strncpy(tmp,i->str,1024);
			winstyle(tmp);

			stradd(" /libpath:\"");
			stradd(tmp);
			stradd("\"");
		}
		i = i->next;
	}
}

void dsp_parse_add_link_flags(void)
{
	item* i = currentConfigIsDebug ?
		link_flags_debug->first 
		:
		link_flags_release->first 
	;
	while (i)
	{
		if (i->str && i->str[0]!=0)
		{
			stradd(" ");
			stradd(i->str);
		}
		i = i->next;
	}
}

void dsp_parse_insert_regular_file( const char* filename )
{
	static char tmp[1024];
	strncpy(tmp,filename,1024);
	winstyle(tmp);
	fprintf(outfile,"# Begin Source File\n\n");
	fprintf(outfile,"SOURCE=%s\n",tmp);
	fprintf(outfile,"# End Source File\n");
}

void dsp_parse_insert_recurse(tree* t,list* repeatcheck, FileType type)
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
			if (cnt>0)
			{
				fprintf(outfile,"# Begin Group \"%s %s No. %d\"\n\n",n->str,typestr,cnt);
			}else{
				fprintf(outfile,"# Begin Group \"%s %s\"\n\n",n->str,typestr);
			}
			dsp_parse_insert_recurse(n->sub,repeatcheck,type);
			fprintf(outfile,"# End Group\n");
		}else{
			if ( type == header )
			{
				assert( mocable_headers != NULL );
				
				if ( list_find( mocable_headers, n->str ) )
					dsp_parse_insert_mocable_header( n->str );
				else
					dsp_parse_insert_regular_file( n->str );
			}
			else if ( type == qt )
			{
				dsp_parse_insert_ui_file( n->str );
			}
			else
				dsp_parse_insert_regular_file( n->str );
		}
		n = n->next;
	}
}


void dsp_parse_insert(FileType type)
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
		typestr = "Qt .ui Files";
		filelist = ui_files;
	}
	else
	{
		typestr = "Unknown files :o";
		filelist = NULL;
	}


	generate_files_tree(filelist, filetree);

	fprintf(outfile,"# Begin Group \"%s\"\n\n",typestr);
	dsp_parse_insert_recurse(filetree,repeatcheck,type);
	fprintf(outfile,"# End Group\n");

	list_free(repeatcheck);
	tree_free(filetree);
}

static void dsp_parse_insert_moc_custom_build_rule( const char* fileString  )
{
	char filename[2048];
	char tmpname[2048];
	char mocname[2048];
	listkey *k = listhash_find( config, "QTDIR" );
	const char* qtdir = k->l->first->str;

	convert_to_mocname( tmpname, 2048, fileString );
	winstyle( tmpname );
	strstart( mocname, 2048 );
	stradd( ".\\");
	stradd( tmpname );
	strend();
	/*Split the header path into the path and the filename */
	discard_path( filename, 2048, fileString );

	strncpy( tmpname, fileString,  2048 );
	winstyle( tmpname );


	/*Generate actual .dsp code */

	fprintf( outfile, "# Begin Custom Build - MOCing %s...\n", filename );
	/* fprintf( outfile, "InputDir=%s\n", pathToFile ); */
	/* fprintf( outfile, "InputPath=%s\n", fileString ); */
	fprintf( outfile, "\n" );
	fprintf( outfile, "\"%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n", mocname );
	fprintf( outfile, "	%s\\bin\\moc.exe %s -o %s \n", qtdir, tmpname, mocname );
	fprintf( outfile, "\n" );
	fprintf( outfile, "# End Custom Build \n" );
}

static void dsp_parse_insert_ui_custom_build_rule( const char* theuifile )
{
	list* outputsList = listhash_find(ui_outputs, theuifile)->l;
	char* outHeader = outputsList->first->str;
	char* outSource = outputsList->first->next->str;
	char* outMoc = outputsList->first->next->next->str;
	char  strippedFile[2048];
	char  strippedHeader[2048];
	char  winStyleUI[2048];
	char  winStyleHeader[2048];
	char  winStyleSource[2048];
	char  winStyleMOC[2048];
	listkey *k = listhash_find( config, "QTDIR" );
	const char* qtdir = k->l->first->str;

	discard_path( strippedFile, 2048, theuifile );
	discard_path( strippedHeader, 2048, outHeader );
	strncpy( winStyleUI, theuifile, 2048 );
	winstyle( winStyleUI );
	strncpy( winStyleHeader, outHeader, 2048 );
	winstyle( winStyleHeader );
	strncpy( winStyleSource, outSource, 2048 );
	winstyle( winStyleSource );
	strncpy( winStyleMOC, outMoc, 2048 );
	winstyle( winStyleMOC );

	/*.dsp code generation*/
	fprintf( outfile, "# Begin Custom Build - UICing %s...\n", strippedFile );
	fprintf( outfile, "\n" );
	fprintf( outfile, "BuildCmds= \\\n");
	fprintf( outfile, "	%s\\bin\\uic.exe %s -o .\\%s \\\n", qtdir, winStyleUI, winStyleHeader );
	fprintf( outfile, "	%s\\bin\\uic.exe %s -i %s -o .\\%s \\\n", qtdir, winStyleUI, strippedHeader, winStyleSource );
	fprintf( outfile, "	%s\\bin\\moc.exe .\\%s -o .\\%s \\\n", qtdir, winStyleHeader, winStyleMOC );
	fprintf( outfile, "\n" );
	fprintf( outfile, "\n" );
	fprintf( outfile, "\".\\%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n", winStyleHeader);
	fprintf( outfile, "   $(BuildCmds)\n");
	fprintf( outfile, "\".\\%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n", winStyleSource);
	fprintf( outfile, "   $(BuildCmds)\n");
	fprintf( outfile, "\".\\%s\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n", winStyleMOC);
	fprintf( outfile, "   $(BuildCmds)\n");
	fprintf( outfile, "\n" );
	fprintf( outfile, "# End Custom Build \n" );
	
}

static void dsp_parse_insert_mocable_header( const char* file  )
{
	char* project_name = 0;
	char tmp[1024];
	
	if ( program && program->first && program->first->str )
	{
		project_name = program->first->str;
	}
	else
		fprintf( stderr, "Error: Variable PROGRAM was not defined!\n" );


	strncpy(tmp,file,1024);
	winstyle(tmp);

	fprintf( outfile, "#Begin Source File\n");
	fprintf( outfile, "\n" );
	fprintf( outfile, "SOURCE=\"%s\"\n", tmp );
	fprintf( outfile, "\n" );
	/*Custom build - release mode */
	fprintf( outfile, "!IF \"$(CFG)\" == \"%s - Win32 Release \"\n", project_name );
	fprintf( outfile, "\n" );
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	dsp_parse_insert_moc_custom_build_rule(  file );
	fprintf( outfile, "\n" );
	/*Custom build - debug mode */
	fprintf( outfile, "!ELSEIF \"$(CFG)\" == \"%s - Win32 Debug\"\n", project_name );
	fprintf( outfile, "\n" );
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	dsp_parse_insert_moc_custom_build_rule(  file );
	fprintf( outfile, "\n" );
	fprintf( outfile, "!ENDIF\n" );
	fprintf( outfile, "\n" );
	fprintf( outfile, "#End Source File\n");
}

static void dsp_parse_insert_ui_file( const char* file  )
{
	char* project_name = 0;
	char tmp[1024];
	
	if ( program && program->first && program->first->str )
	{
		project_name = program->first->str;
	}
	else
		fprintf( stderr, "Error: Variable PROGRAM was not defined!\n" );


	strncpy(tmp,file,1024);
	winstyle(tmp);

	fprintf( outfile, "#Begin Source File\n");
	fprintf( outfile, "\n" );
	fprintf( outfile, "SOURCE=\"%s\"\n", tmp );
	fprintf( outfile, "\n" );
	/*Custom build - release mode */
	fprintf( outfile, "!IF \"$(CFG)\" == \"%s - Win32 Release \"\n", project_name );
	fprintf( outfile, "\n" );
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	dsp_parse_insert_ui_custom_build_rule(  file );
	fprintf( outfile, "\n" );
	/*Custom build - debug mode */
	fprintf( outfile, "!ELSEIF \"$(CFG)\" == \"%s - Win32 Debug\"\n", project_name );
	fprintf( outfile, "\n" );
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	dsp_parse_insert_ui_custom_build_rule(  file );
	fprintf( outfile, "\n" );
	fprintf( outfile, "!ENDIF\n" );
	fprintf( outfile, "\n" );
	fprintf( outfile, "#End Source File\n");
}

void dsp_parse_insert_settings_rule(void)
{
	char* project_name = 0;
	if (program && program->first && program->first->str)
	{
		project_name = program->first->str;
	}
	else
	{
		fprintf(stderr,"Error: variable PROGRAM not defined\n");	
	}
	fprintf(outfile,"# Begin Source File\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"SOURCE=settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\n",
		project_name);
	fprintf(outfile,"\n");
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	fprintf(outfile,"# Begin Custom Build\n");
	fprintf(outfile,"InputPath=settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile, "\"buildstamp\" : $(SOURCE) \"$(INTDIR)\"\n" );
	fprintf(outfile, "	srcdeps.exe settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Custom Build\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",
		project_name);
	fprintf(outfile,"\n");
	fprintf( outfile, "# PROP Ignore_Default_Tool 1\n");
	fprintf(outfile,"# Begin Custom Build\n");
	fprintf(outfile,"InputPath=settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile, "\"buildstamp\" : $(SOURCE) \"$(INTDIR)\"\n" );
	fprintf(outfile, "	srcdeps.exe settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Custom Build\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"!ENDIF \n");
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Source File\n");
}

void dsp_parse_insert_sources()
{
	dsp_parse_insert(source);
}

void dsp_parse_insert_headers()
{
	dsp_parse_insert(header);
}

void dsp_parse_insert_ui_rules()
{
	dsp_parse_insert(qt);
}

/*
void copy_without_includes(char* tgt,const char* src)
{
	while (*src && *src!='\r' && *src!='\n')
	{
		if (!strcmp_begin(src,"/I \""))
		{
			src += 4;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/I\""))
		{
			src += 3;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/D \""))
		{
			src += 4;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}
		else if (!strcmp_begin(src,"/FI\""))
		{
			src += 4;
			while (*src && *src!='"') src++;
			if (*src=='"') src++;
			if (*src==' ') src++;
		}else{
			*tgt++ = *src++;
		}
	}
	*tgt = 0;
}
*/

int dsp_parser_modify_project_name(const char* buf,char* tmp,int len)
{
	char* project_name = 0;
	if (program && program->first && program->first->str)
	{
		project_name = program->first->str;
	}
	else
	{
		fprintf(stderr,"Error: variable PROGRAM not defined\n");	
	}
	
	{
		char* cmp="# Microsoft Developer Studio Project File - Name=\"";
		if (!strcmp_begin(buf,cmp))
		{
			const char* a = buf;
			strstart(tmp,len);
			stradd(cmp);
			a += strlen(cmp);
			while (*a && *a!='"') a++;
			stradd(project_name);
			stradd(a);
			strend();
			return 1;
		}
	}

	{
		char* cmps[]={
			"CFG=",
			"!IF  \"$(CFG)\" == \"",
			"!ELSEIF  \"$(CFG)\" == \"",
			"!MESSAGE \"",
			"# Name \"",
			0};
		int i = 0;
		char* cmp;
		while ((cmp=cmps[i])!=0)
		{
			if (!strcmp_begin(buf,cmp))
			{
				const char* a = buf;
				a += strlen(cmp);
				a = strstr(a," - ");
				if (a)
				{
					strstart(tmp,len);
					stradd(cmp);
					stradd(project_name);
					stradd(a);
					strend();
					return 1;
				}
			}
			i++;
		}
	}

	{
		char* cmp="!MESSAGE NMAKE /f \"";
		if (!strcmp_begin(buf,cmp))
		{
			const char* a = buf;
			const char* aa;
			char* cmp2 = ".mak\".";
			
			aa = strstr(a,cmp2);
			if (aa)
			{
				strstart(tmp,len);
				stradd(cmp);
				stradd(project_name);
				stradd(aa);
				strend();
				return 1;
			}

			cmp2 = ".mak\" CFG=\"";

			aa = strstr(a,cmp2);
			if (aa) {
				aa = strstr(a," - ");
				if (aa)
				{
					strstart(tmp,len);
					stradd(cmp);
					stradd(project_name);
					stradd(cmp2);
					stradd(project_name);
					stradd(aa);
					strend();
					return 1;
				}
			}
		}
	}

	return 0;	
}

void dsp_parse_line_chkcfg(const char* buf,int line)
{
	const char* a = 0;
	if (strcmp_begin(buf,"!ELSEIF  \"$(CFG)\" == \"")==0)
	{
		a = buf + strlen("!ELSEIF  \"$(CFG)\" == \"");
	}
	else if (strcmp_begin(buf,"!IF  \"$(CFG)\" == \"")==0)
	{
		a = buf + strlen("!IF  \"$(CFG)\" == \"");
	}
	
	if (a)
	{
		int n = 256;
		char* b = currentConfig;
		while (--n && *a && *a!='"') { *b++ = *a++;}
		*b = 0;

		b = currentConfig;
		while (*b) {
			if (strcmp("Debug",b)==0){
				currentConfigIsDebug = 1;
				break;
			}
			b++;
		}
		if (!*b) 
			currentConfigIsDebug = 0;

/*		fprintf(stderr,"%d CURRENTCONFIG=%s %d\n",line,currentConfig,currentConfigIsDebug);
*/
	}
	if (strcmp_eol(buf,"!ENDIF")==0)
	{
		strcpy(currentConfig,"");
		currentConfigIsDebug = 0;
	}
}

void dsp_parse_line(const char* buf,int line)
{
	dsp_parse_line_chkcfg(buf,line);

	if (sourcefile==1)
	{
		if (strcmp_eol(buf,""))  /* ignore empty lines */
		{
			if (strncmp(buf,"SOURCE=",7))
			{
				fprintf(stderr,
	"Error: expected SOURCE=... after # Begin Source File in line %d\n",line);
				fprintf(stderr,buf);
				exit(-1);
			}
			sourcefile = 2;
			if (!strcmp_eol(buf+7,"settings.cfg"))
			{
				skipsource = 1;
			}
			else
			{
				fprintf(outfile,"# Begin Source File\n\n");
			}
		}
	}

	if (!skip)
	{
		if (!strcmp_eol(buf,"# Begin Source File"))
		{
			sourcefile = 1;
		}
	}

	if (!strcmp_begin(buf,begingroup))
	{
		char groupname[1024];
		const char* a = buf;
		char* b = groupname;
		char* q = 0;
		if (begingroupl==0) begingroupl=strlen(begingroup);
		a += begingroupl;
		while (*a)
		{
			if (*a=='"') q = b;
			*b++ = *a++;
		}
		if (!q) {
			fprintf(stderr,"Did not find terminating '\"' in line %d\n",line);
			exit(-1);
		}
		*q = 0;
		stack_push(groupstack,groupname);
		if (!strcmp(groupname,"Source Files"))
		{
			skip = 1;
		}
		if (!strcmp(groupname,"Header Files"))
		{
			skip = 1;
		}
	}
	
	if (!skip && !skipsource && sourcefile!=1) {
		char tmp[4096];
		if (
			!strcmp_begin(buf,"# ADD CPP ")
		) {
			/*
			char tmp2[4096];
			copy_without_includes(tmp2,buf);
			strstart(tmp,4096);
			stradd(tmp2);
			*/

			strstart(tmp,4096);
			stradd("# ADD CPP");
			
			/** add all needed_includepaths here **/

			dsp_parse_add_cxxflags();
			dsp_parse_add_defines();
			dsp_parse_add_pre_includes();
			dsp_parse_add_needed_includepaths();

			stradd("\n");
			
			strend();
			fputs(tmp,outfile);
		}
		else
		if (
			!strcmp_begin(buf,"# ADD LINK32")
		) {
			/*
			char tmp2[4096];
			copy_without_includes(tmp2,buf);
			strstart(tmp,4096);
			stradd(tmp2);
			*/

			strstart(tmp,4096);
			stradd("# ADD LINK32");
			
			/** add all needed_includepaths here **/

			dsp_parse_add_library_paths();

			dsp_parse_add_libraries();

			dsp_parse_add_link_flags();

			stradd("\n");
			
			strend();
			fputs(tmp,outfile);
		}
		else
		if (dsp_parser_modify_project_name(buf,tmp,4096))
		{
			fputs(tmp,outfile);
		}
		else
		{
			if (!strcmp_eol(buf,"# End Target"))
			{
				char tmpname[2048];
				char tmpname2[2048];

				if ( mocable_headers)
				{
					item* current = mocable_headers->first;				
					while ( current != NULL )
					{		
						convert_to_mocname( tmpname, 2048, current->str );
						strstart( tmpname2, 2048 );
						stradd("./");
						stradd(tmpname);
						strend();

						list_add_str_once( guessed_sources, tmpname2 );
						current = current->next;
					}
				}

				/* And now we just insert the files resulting from UICing */
				{
					if ( listhash_find( config, "UI_FILES") )
					{
						listkey* currentEntry = ui_outputs->first;
						char tmpname[2048];
						
						while( currentEntry != NULL )
						{
							item* currentOutput = currentEntry->l->first;
							/*ui generated header*/
							strstart( tmpname, 2048 );
							stradd( "./" );
							stradd( currentOutput->str );
							strend();
							list_add_str_once( guessed_headers, tmpname );

							/* ui generated source */
							currentOutput = currentOutput->next;
							strstart( tmpname, 2048 );
							stradd( "./" );
							stradd( currentOutput->str );
							strend();
							list_add_str_once( guessed_sources, tmpname );

							/* moc resulting from ui generated source */
							currentOutput = currentOutput->next;
							strstart( tmpname, 2048 );
							stradd( "./" );
							stradd( currentOutput->str );
							strend();
							list_add_str_once( guessed_sources, tmpname );

							currentEntry = currentEntry->next;
						}
					}
				}

				dsp_parse_insert_sources();
				dsp_parse_insert_headers();
				dsp_parse_insert_ui_rules();
				dsp_parse_insert_settings_rule();
			}	
			fputs(buf,outfile);
		}
	}
	
	if (!strcmp_eol(buf,endgroup))
	{	
		if (skip && !strcmp(stack_top(groupstack),"Source Files")) skip = 0;
		if (skip && !strcmp(stack_top(groupstack),"Header Files")) skip = 0;
		stack_pop(groupstack);
	}

	if (!strcmp_eol(buf,"# End Source File"))
	{
		sourcefile = 0;
		if (skipsource) skipsource = 0;
	}
}

