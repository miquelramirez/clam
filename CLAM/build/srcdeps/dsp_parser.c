#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "config_parser.h"
#include "listhash.h"
#include "tree.h"
#include "stack.h"
#include "list.h"
#include "strfuncs.h"

stack* groupstack = 0;

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

void winstyle(char* str)
{
	while (*str)
	{
		if (*str=='/') *str='\\';
		str++;
	}
}

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

FILE* infile = 0;
FILE* outfile = 0;

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

void dsp_parse_insert_recurse(tree* t,list* repeatcheck,int type)
{
	node * n = t->first;
	char* typestr = type ? "Headers" : "Sources";
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
			char tmp[1024];
			strncpy(tmp,n->str,1024);
			winstyle(tmp);
			fprintf(outfile,"# Begin Source File\n\n");
			fprintf(outfile,"SOURCE=%s\n",tmp);
			fprintf(outfile,"# End Source File\n");
		}
		n = n->next;
	}
}


void dsp_parse_insert(int type)
{
	tree* t = tree_new();
	item* i = type ? guessed_headers->first : guessed_sources->first;
	list* repeatcheck = list_new();

	char* typestr = type ? "Header Files" : "Source Files";

	while (i)
	{
		char tmp[1024];
		char* ptr;
		char* start = 0;

		tree* c = t;

		strncpy(tmp,i->str,1024);
		
		ptr = tmp;

		while (*ptr)
		{
			if (start==0) start = ptr;
			if (*ptr=='/' || *ptr=='\\')
			{
				*ptr = 0;
				if (strcmp(start,"..") && strcmp(start,"src"))
				{
					node* n = tree_add_str_once(c,start);
					if (!n->sub) n->sub = tree_new();
					c = n->sub;
				}
				start = 0;
			}
			ptr++;
		}

		tree_add_str_once(c,i->str);
		
		i = i->next;
	}

	fprintf(outfile,"# Begin Group \"%s\"\n\n",typestr);
	dsp_parse_insert_recurse(t,repeatcheck,type);
	fprintf(outfile,"# End Group\n");

	list_free(repeatcheck);
	tree_free(t);
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
	fprintf(outfile,"# Begin Custom Build\n");
	fprintf(outfile,"InputPath=settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"\"buildstamp\" : $(SOURCE) \"$(INTDIR)\" \"%s.dsp\"\n",
		project_name);
	fprintf(outfile,"	srcdeps.exe settings.cfg %s.dsp\n",
		project_name);
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Custom Build\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\n",
		project_name);
	fprintf(outfile,"\n");
	fprintf(outfile,"# Begin Custom Build\n");
	fprintf(outfile,"InputPath=settings.cfg\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"\"buildstamp\" : $(SOURCE) \"$(INTDIR)\" \"%s.dsp\"\n",
		project_name);
	fprintf(outfile,"	srcdeps.exe settings.cfg %s.dsp\n",
		project_name);
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Custom Build\n");
	fprintf(outfile,"\n");
	fprintf(outfile,"!ENDIF \n");
	fprintf(outfile,"\n");
	fprintf(outfile,"# End Source File\n");
}

void dsp_parse_insert_sources()
{
	dsp_parse_insert(0);
}

void dsp_parse_insert_headers()
{
	dsp_parse_insert(1);
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
			!strcmp_begin(buf,"# ADD LINK32 ")
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
				dsp_parse_insert_sources();
				dsp_parse_insert_headers();
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

void dsp_parse(const char* filename)
{
	char buf[4096];
	int line = 0;

	groupstack = stack_new();

	infile = fopen(filename,"r");
	if (!infile) {
		fprintf(stderr,"Could not open %s for reading\n",filename);
		exit(-1);
	}
	outfile = fopen("PARSETMP.dsp","w");
	if (!outfile)
	{
		fprintf(stderr,"Could not open PARSETMP.dsp for writing\n");
		exit(-1);
	}
	while (fgets(buf,4096,infile))
	{
		line++;
		dsp_parse_line(buf,line);
	}
	
	fclose(infile);
	fclose(outfile);

	infile = fopen("PARSETMP.dsp","r");
	if (!infile) {
		fprintf(stderr,"Could not open PARSETMP.dsp for reading\n");
		exit(-1);
	}
	outfile = fopen(filename,"w");
	if (!outfile)
	{
		fprintf(stderr,"Could not open %s for writing\n",filename);
		exit(-1);
	}

	while (fgets(buf,4096,infile))
	{
		fputs(buf,outfile);
	}

	fclose(infile);
	fclose(outfile);
	
	remove("PARSETMP.dsp");
}
