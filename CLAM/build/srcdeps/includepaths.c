#include "list.h"
#include "recursedir.h"
#include "strfuncs.h"
#include "strptr.h"
#include "includepaths.h"
#include <stdlib.h>
#include <string.h>

extern list* includepaths; /* definition in parser.c */

/* add a path to the include paths */
/* Don't add /CVS paths! */
void includepaths_add(const char* str)
{
	const char* ptr = str;
	ptr = strptr_skip_end(ptr);
	while (ptr!=str && *ptr!='/' && *ptr!='\\') ptr--;
	if (*ptr=='\\') 
	{
		fprintf(stderr, "Error in includepaths.c: '\\' encountered in a include path\n line:%s \n ",str);
		exit(-1);
	}
	if ((*ptr=='/') && !strcmp(ptr,"/CVS") ) return;

	list_add_str_once(includepaths,str);
}


/* Callback for recursedir to call on each directory, see includepaths_add_rec */
int cb_includepaths_add(const char* dir,void*dum)
{
	includepaths_add(dir);
	return 0;
}

/* Recurse a directory tree and add all subdirs to the includepaths list */
void includepaths_add_rec(const char* str)
{
	recursedir(str,-1,0,cb_includepaths_add,0);
}

/* Look for a filename in all includepaths, and open it if found.
** Returns NULL or the filehandle. pathret is set to the path where
** the file was found.
*/
FILE* fopen_in_includepaths(const char* filename,const char** pathret)
{
	item* i = includepaths->first;
	while (i)
	{
		FILE* ret;
		char tmp[2048];
		strstart(tmp,2048);
		stradd(i->str);
		stradd("/");
		stradd(filename);
		strend();
		ret = fopen(tmp,"r");
		if (ret) {
			*pathret = i->str;
			return ret;
		}
		i = i->next;
	}

	return 0;
}
