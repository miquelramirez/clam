#include "objdepname.h"
#include "strptr.h"
#include "strfuncs.h"
#include <string.h>

/* Return a pointer to the extension in a filename (the last dot) 
** or NULL if extension was found
*/
char* extension(char* filename)
{
	char* ptr = filename;
	ptr = (char*)(strptr_skip_end(ptr));
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='.') return ptr;
	}
	return 0;
}


void convert_to_objname(char* objname,int maxlen,const char* filename)
{
	const char* ptr = filename;
	ptr = strptr_skip_end(ptr);
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='/') { ptr++; break; }
	}
	strstart(objname,maxlen-3); /* leave space for extension */
	stradd("obj/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(objname);
		strncpy(ext,".o",3);
	}
}

void convert_to_depname(char* depname,int maxlen,const char* filename)
{
	const char* ptr = filename;
	ptr = strptr_skip_end(ptr);
	while (ptr!=filename)
	{
		ptr--;
		if (*ptr=='/') { ptr++; break; }
	}
	strstart(depname,maxlen-3); /* leave space for extension */
	stradd("dep/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(depname);
		strncpy(ext,".d",3);
	}
}
