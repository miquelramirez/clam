#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "config_parser.h"
#include "list.h"
#include "hash.h"
#include "stack.h"
#include "strfuncs.h"
#include "strptr.h"
#include "conditions.h"
#include "includepaths.h"
#include "objdepname.h"

/* for mkdir */
#ifdef WIN32
#include <direct.h>
#else

#include <sys/stat.h>
#include <sys/types.h>

#endif

#include "verbose.h" 

/* hash with all cur_defines during a parser_run */
hash *cur_defines = 0;

/* list of include files found during a parser_run */
list *includes = 0;

/* list of files to pre-include in each parser_run, 
** set with command line option -include 
*/
list *pre_includes = 0;

/* list of define to pre-define in each parser_run, 
** set with command line option -D 
*/
list *defines = 0;

/* list of paths to search include files.
** set with command line options -I (add a single path)
** and -R (add a path and all its subdirectories)
*/
list *includepaths = 0;

/* list of the sources and estimated sources to parse
*/
list *guessed_sources = 0;

list *guessed_headers = 0;

/* list of include paths that are actually needed to 
** compile the guessed_sources
*/
list *needed_includepaths = 0;

/* list of all includes checked when building the
** needed_includepaths, for efficiency.
*/
list *includes_checked = 0;

/* stack of the files visited during recursion, used for
** error reporting
*/
stack* filenamestack = 0;

/* flag whether to estimate required source for included headers
*/
int recursesrcs = 0;

/* flag whether to generate object (.o) and depend (.d) file on source/header
** dependencies
** 0 = don't generate depend (for visual dsp's)
** 1 = generate single file dependency
** 2 = generate full dependencies (including seperate .d files) (option -f)
*/
int gendepend = 0;



/* hash of header extension -> source extension,
** used to estimate required source for included
** headers if recursesrc is set
*/
hash* extmap = 0;

void extmap_exit(void)
{
	hash_free(extmap);
}

void extmap_init(void)
{
	extmap = hash_new();
	
	/* these are the most logical */
	hash_add(extmap,".h",".c");
	hash_add(extmap,".hh",".cc");
	hash_add(extmap,".hxx",".cxx");
	hash_add(extmap,".hpp",".cpp");
	hash_add(extmap,".H",".C");
	
	/* these are weird, but also occur */
	hash_add(extmap,".h",".cc");
	hash_add(extmap,".h",".cxx");
	hash_add(extmap,".h",".cpp");
	hash_add(extmap,".h",".C");

	hash_add(extmap,".H",".cc");
	hash_add(extmap,".H",".cxx");
	hash_add(extmap,".H",".cpp");
	hash_add(extmap,".H",".C");

	hash_add(extmap,".hpp",".cxx");
	hash_add(extmap,".hxx",".cpp");
}



/* Check is a file exists */
int file_exists(const char* filename)
{
	FILE* f = fopen(filename,"r");
	if (f) {
		fclose(f);
		return 1;
	}
	return 0;
}

int parser_recurse(const char* filename);

/* Handle the filename as parsed from a #include directive:
** - look for the file in all includepaths
** - parse it recursively
** - add the path where it was found to the list of needed includepaths
** - if recursesrcs flag is set, look for the implementation source
**   trying different extensions occurding to extmap, and add that
**   source to the sources list
*/
int parser_include(const char* filename)
{
	const char* path;
	char tmp[2048];
	char tmp2[2048];
	char* pathend = 0;
	int inlocalpath = 0;

	/* about tmp vs tmp2: 
	** we used to add tmp to includes_checked, but
	** this went wrong when the a different path and 
	** a different include let to the same file,
	** for example PATH a/b , INCLUDE c and
	** PATH a INCLUDE b/c
	** so now we use tmp2 where path and file are
	** seperated with /:/ instead of only a /
	*/

	const char* curFilename = stack_top(filenamestack);
	
	FILE* f = 0;

	if (verbose)
		fprintf(stderr,"Including %s from %s\n",filename,curFilename);
		
	{
		/* first, check local path */
		const char* a = curFilename;
		char* b = tmp;
		char* c = tmp2;
		char* qb = 0;
		char* qc = 0;
		int n = 2032; /* leave room for possible extension change */
		while (*a && n)
		{
			if (*a=='/' || *a=='\\')
			{
				qb = b;
				qc = c;
			}
			*b++ = *a;
			*c++ = *a;
			a++;
			n--;
		}
		*b = 0;
		*c = 0;
		if (qb)
		{
			pathend = qc;
			qb++;
			qc++;
			*qc++ = ':';
			*qc++ = '/';
			strncpy(qb,filename,n);
			strncpy(qc,filename,n-2);
		}
		
		f = fopen(tmp,"r");
	}

	if (f==0)
	{
		/* the file was not in the local path, so we check all other paths */
		f = fopen_in_includepaths(filename,&path);
		if (f)
		{
			if (verbose)
				fprintf(stderr,"Found in path %s\n",path);

			strstart(tmp,2032); /* leave room for possible extension change */
			stradd(path);
			if (strcmp(path,""))
			{
				stradd("/");
			}
			stradd(filename);
			strend();	

			strstart(tmp2,2048);
			pathend = stradd(path);
			stradd("/:/");
			stradd(filename);
			strend();	
		}
	}else{
		if (verbose)
			fprintf(stderr,"Found in same directory\n");
		inlocalpath = 1;
	}

	if (!f) 
	{
		if (verbose)
			fprintf(stderr,"Not found...\n");
		return 0;
	}else{
		fclose(f);
	}

	/* now, :
	** - tmp contains the path + "/" +  filename
	** - tmp2 	contains the path + "/:/" + filename
	** - pathend point to the end of the path in tmp2
	*/

	parser_recurse(tmp);

	list_add_str_once(includes,tmp);

	if (!list_find(includes_checked,tmp2))
	{
		item* i = extmap->l->first;
		
		list* possible_impl_files = list_new();

		if (!inlocalpath)
		{
			list_add_str(includes_checked,tmp2);
		}
			
		/* remove filename from tmp2 */
		*pathend = 0;

		if (!inlocalpath)
		{
			/* the compiler will look in the local path
			** anyway, so don't add it to the list of
			** needed include paths */
#ifndef WIN32
			if (	strcmp(tmp2,"/usr/include") && 
				strcmp(tmp2,"/usr/local/include") )
			{
				if (verbose)
					fprintf(stderr,"Adding %s to needed include paths",tmp2);
				list_add_str_once(needed_includepaths,tmp2);
			}
#else
			list_add_str_once(needed_includepaths,tmp2);
#endif
		}
		list_add_str_once(guessed_headers,tmp);

		if (recursesrcs)
		{
			char* ext = extension(tmp);
			char  headerext[16];
			strncpy(headerext,ext,16);

			while (i)
			{
				/* try various extensions */
				const char* map = hash_item_value(extmap,i,headerext);
				if (map)
				{	
					strncpy(ext,map,16);
					if (file_exists(tmp))
					{
						list_add_str_once(possible_impl_files,tmp);
					}
				}
				i = i->next;
			}
	
			strcpy(ext,headerext); /* reset filename to header */

			{
				int n = list_size(possible_impl_files);
				if (n>1)
				{
					item* k = possible_impl_files->first;
					fprintf(stderr,"Multiple possible implementations for %s:\n",tmp);
					while (k)
					{
						fprintf(stderr,"  %s\n",k->str);
						k = k->next;
					}
				}
				if (n==1)
				{
					list_add_str_once(guessed_sources,possible_impl_files->first->str);
				}
			}
		}						
		list_free(possible_impl_files);
	}else{
		if (verbose)
			fprintf(stderr,"Already checked\n");
	}	
	return 1;
}

/* parse a preprocessor directive. ptr is at the start of the directive */
const char* parser_directive(const char* ptr)
{
	if (conditions_total()) 
	/* only do includes, defines and undefs if the 
	** conditions total is true */
	{
		if (strncmp(ptr,"include",7)==0)
		{
			char term;
			char tmp[2048];

			ptr+=7;

parse_include_filename: 
			ptr = strptr_skip_spaces(ptr);
			term = *ptr;
			if (term=='"') {
				term='"'; ptr++;
			}
			else if (term=='<') {
				term='>'; ptr++;
			}
			else term = 0;
			if (term==0)
			{
				ptr = strptr_copy_nonspaces(ptr,tmp,2048);

				{
					/* this goto is a bit of a kludge to handing defines in includes */
					const char* val = hash_value(cur_defines,tmp);
					if (val)
					{
						strncpy(tmp,val,2048);
						ptr = tmp; 
						goto parse_include_filename;
					}else{
						fprintf(stderr,"Error: include %s syntax error\n",tmp);
					}
				}
			}
			else
			{
				ptr = strptr_copy_until(ptr,term,tmp,2048);
			}

			if (parser_include(tmp)==0)
			{
				if (term=='>') {
					/* a system include was not found, but that's okay, because
					** we don't generate dependencies for system includes anyway
					*/
				}
				else
				{
					{
						const char* i = 0;
						fprintf(stderr,"Error: include \"%s\" not found\n",tmp);

						while ((i=stack_top(filenamestack)))
						{
							fprintf(stderr,"  from %s\n",i);
							stack_pop(filenamestack);
						}
					}
										
					fprintf(stderr,"Checked in:\n");
					{
						item* i = includepaths->first;
						while (i)
						{
							fprintf(stderr,"  %s\n",i->str);
							i = i->next;
						}
					
					}
					exit(-1);
				}
			}
			return ptr;
		}
		if (strncmp(ptr,"define",6)==0)
		{
			char key[256];
			char val[256];
			ptr+=6;
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,key,256);
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,val,256);
			if (verbose) fprintf(stderr,"defining %s = %s\n",key,val);
			hash_set(cur_defines,key,val);
			return ptr;
		}
		if (strncmp(ptr,"undef",5)==0)
		{
			char key[256];
			ptr+=5;
			ptr = strptr_skip_spaces(ptr);
			ptr = strptr_copy_nonspaces(ptr,key,256);
			hash_rmv(cur_defines,key);
			return ptr;
		}
	}
	if (strncmp(ptr,"ifdef",5)==0)
	{
		char key[256];
		int c;
		ptr+=5;
		ptr = strptr_skip_spaces(ptr);
		ptr = strptr_copy_nonspaces(ptr,key,256);
		
		c = hash_value(cur_defines,key)!=0;
		if (verbose) fprintf(stderr,"ifdef %s = %d\n",key,c);
		conditions_push(c);
		return ptr;
	}
	if (strncmp(ptr,"ifndef",6)==0)
	{
		char key[256];
		int c;
		ptr+=6;
		ptr = strptr_skip_spaces(ptr);
		ptr = strptr_copy_nonspaces(ptr,key,256);

		c = hash_value(cur_defines,key)==0;
		if (verbose) fprintf(stderr,"ifndef %s = %d\n",key,c);
		conditions_push(c);
		return ptr;
	}
	if (strncmp(ptr,"if",2)==0)
	{
		ptr+=2;
		if (verbose) fprintf(stderr,"if\n");
do_if:
		/* TODO! We should parse the #if preprop.directive as well */
		conditions_push(0);
	}
	if (strncmp(ptr,"elif",4)==0)
	{
		ptr+=4;
		if (verbose) fprintf(stderr,"elif\n");
		conditions_pop();
		goto do_if;
	}
	if (strncmp(ptr,"else",4)==0)
	{
		ptr+=4;
		if (verbose) fprintf(stderr,"else\n");
		conditions_invert();
		return ptr;
	}
	if (strncmp(ptr,"endif",5)==0)
	{
		ptr+=5;
		if (verbose) fprintf(stderr,"endif\n");
		conditions_pop();
		return ptr;
	}
	return ptr;
}

/* Parse ptr and return after the end of a C-style comment, 
** or the end of the line, or NULL of we are still in the comment
*/
const char* parser_comment(const char* ptr)
{
	while (*ptr)
	{
		if (*ptr=='*' && *(ptr+1)=='/') return ptr+2;
		ptr++;
	}
	return 0;
}

/* parse a line. return 1 if after parsing the line we are still inside 
** a c-style comment */
int parser_line(const char* ptr)
{
restart:
	ptr = strptr_skip_spaces(ptr);
	if (*ptr=='/' && *(ptr+1)=='/') return 0;
	if (*ptr=='/' && *(ptr+1)=='*') 
	{
		ptr = parser_comment(ptr+2);
		if (ptr==0) {
			return 1;
		}
		goto restart;
	}
	if (*ptr=='#')
	{
		ptr++;
		ptr = strptr_skip_spaces(ptr);
		if (*ptr)
		{
			ptr = parser_directive(ptr);
		}
	}
	return 0;
}

int parser_recurse(const char* filename)
{
	static FILE* f = 0;
	FILE* g = fopen(filename,"r");
	int in_comment = 0;
	static char buf[2048];
	int filepos = -1;

	if (g==0) return 0;

	if (f)
	{
		filepos = ftell(f);
//		fprintf(stderr,"saving filepos %s:%d\n",stack_top(filenamestack),filepos);
		fclose(f);
	}


	f = g;
	
	stack_push(filenamestack,filename);

//	fprintf(stderr,"opening %s\n",filename);

	while (fgets(buf, 2048, f))
	{
		const char* ptr = buf;
		//fprintf(stderr,buf);
		if (in_comment)
		{
			ptr = parser_comment(ptr);
			if (ptr) in_comment = 0;
		}
		if (!in_comment)
		{
			in_comment = parser_line(ptr);
		}
	}
	
	fclose(f);

	stack_pop(filenamestack);

//	fprintf(stderr,"closing %s\n",filename);

	if (filepos != -1)
	{
//		fprintf(stderr,"restore filepos %s %d\n",stack_top(filenamestack),filepos);
		f = fopen(stack_top(filenamestack),"r");
		fseek(f,filepos,SEEK_SET);
	}else
	{
		f = 0;
	}

	return 1;
}

void parser_init(void)
{
	guessed_sources = list_new();

	guessed_headers = list_new();

	includepaths = list_new();

	includes_checked = list_new();

	needed_includepaths = list_new();

	{
		listkey* k = listhash_find(config,"SOURCES");
		list* sources = k->l;
		item* i = sources->first;
		while (i)
		{
			list_add_str_once(guessed_sources,i->str);
			i = i->next;
		}
	}

	pre_includes = listhash_find(config,"PRE_INCLUDES")->l;

	defines = listhash_find(config,"DEFINES")->l;

	extmap_init();

	{
		list* search_includes = listhash_find(config,"SEARCH_INCLUDES")->l;
		list* search_recurse_includes = listhash_find(config,"SEARCH_RECURSE_INCLUDES")->l;

		item* i;

		i = search_includes->first;
		while (i)
		{
			includepaths_add(i->str);
			i = i->next;
		}
#ifndef WIN32
/* MDB: TODO: I really prefer not to include these, because it really slows
** down srcdeps, especially on OSX, but maybe some problem occurs???
		includepaths_add("/usr/include");
		includepaths_add("/usr/local/include");
*/
#endif
		i = search_recurse_includes->first;
		while (i)
		{
			includepaths_add_rec(i->str);
			i = i->next;
		}
	}
}

void parser_exit(void)
{
	list_free(guessed_sources);

	list_free(guessed_headers);

	list_free(includepaths);

	list_free(includes_checked);

	list_free(needed_includepaths);
	
	extmap_exit();
}

void parser_run(const char* filename)
{
	if (verbose) fprintf(stderr,"create defines hash\n");

	cur_defines = hash_new();

	includes = list_new();

	filenamestack = stack_new();

	conditions_start();

	{
		item* i = defines->first;
		while (i)
		{
			hash_set(cur_defines,i->str,"");
			i = i->next;
		}
	}
	
	{
		item* i = pre_includes->first;
		while (i)
		{
			parser_recurse(i->str);
			i = i->next;
		}
	}

	if (parser_recurse(filename))
	{
		/* {	
			item* i = includes->first;
			while (i)
			{
				parser_recurse(i->str,1);
				i = i->next;		
			}
		} */

		if (gendepend==1)
		{
			item* i;
			char objname[2048];
			char depname[2048];

			i = includes->first;

			convert_to_objname(objname,2048,filename);

			printf("%s: %s ",objname,filename);
			while (i)
			{
				printf("\\\n %s",i->str);

				i = i->next;
			}
			printf("\n\n");

			i = includes->first;

			convert_to_depname(depname,2048,filename);

			printf("%s:",depname);
			while (i)
			{
				printf("\\\n %s",i->str);

				i = i->next;
			}
			printf("\n\n");
		}
		else
		if (gendepend==2)
		{
			char depname[2048];
			FILE* df;
			convert_to_depname(depname,2048,filename);

			printf("%s: %s \n\n",depname,filename);
			
#ifdef WIN32
			mkdir("dep");
#else
			mkdir("dep",0775);
#endif		
			df = fopen(depname,"w");
			
			if (df == 0)
			{
				fprintf(stderr,"Failed to open %s for writing\n",depname);
				exit(-1);
			}
			else
			{
				item* i;
				char objname[2048];
				char depname[2048];

				i = includes->first;

				convert_to_objname(objname,2048,filename);

				fprintf(df,"%s: %s ",objname,filename);
				while (i)
				{
					fprintf(df,"\\\n %s",i->str);

					i = i->next;
				}
				fprintf(df,"\n\n");

				i = includes->first;

				convert_to_depname(depname,2048,filename);

				fprintf(df,"%s:",depname);
				while (i)
				{
					fprintf(df,"\\\n %s",i->str);

					i = i->next;
				}
				fprintf(df,"\n\n");
			}
			
			fclose(df);
			
		}
	}else{
		fprintf(stderr,"Could not open %s\n",filename);
	}
	if (verbose) fprintf(stderr,"free defines hash\n");
	hash_free(cur_defines);
	
	list_free(includes);
	stack_free(filenamestack);

	conditions_end();
}


