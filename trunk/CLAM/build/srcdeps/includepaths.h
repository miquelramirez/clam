#ifndef __includepaths__
#define __includepaths__

#include <stdio.h>

/* add a path to the include paths */
/* Don't add /CVS paths! */
void includepaths_add(const char* str);

/* Recurse a directory tree and add all subdirs to the includepaths list */
void includepaths_add_rec(const char* str);

/* Look for a filename in all includepaths, and open it if found.
** Returns NULL or the filehandle. pathret is set to the path where
** the file was found.
*/
FILE* fopen_in_includepaths(const char* filename,const char** pathret);

#endif


