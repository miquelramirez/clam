#include "objdepname.h"
#include "strptr.h"
#include "strfuncs.h"
#include <string.h>
#include <assert.h>

/* Return a pointer to the extension in a filename (the last dot) 
** or NULL if extension was found
*/
char* extension( char* filename)
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
	const char* ptr = split_path( filename );

	strstart(objname,maxlen-3); /* leave space for extension */
	stradd("obj/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(objname);
		strncpy(ext,".o",3);
	}
}

const char* split_path( const char* filename )
{
	const char * ptr = filename;
	ptr = strptr_skip_end( ptr );

	while( ptr != filename )
	{
		ptr--;
		if ( *ptr == '/' )
		{
			ptr++;
			break;
		}
	}
	
	return ptr;
}

void extract_path( char* path, int maxLen, const char* filename )
{

	const char* ptr = split_path( filename );
	
	strstart( path, maxLen );
	stradd_range( filename, ptr );
	strend();
}

void convert_to_mocname(char* mocname, int maxlen, const char* filename )
{
	char* extPos = extension( (char*)filename );
	
	const char* ptr = split_path( filename );

	assert( extPos - ptr > 0 );
	
	assert( ( extPos - ptr + 12 ) <= maxlen );

	strstart( mocname, maxlen );
	stradd( "moc/" );
	stradd_range( ptr, extPos );
	stradd( "_moc.cxx" );
	strend();
		
}

void discard_path( char* newName, int maxlen, const char* filename )
{
	const char* ptr = split_path( filename );

	strstart( newName, maxlen );
	stradd( ptr );
	strend();
}

void convert_to_depname(char* depname,int maxlen,const char* filename)
{
	
	const char* ptr = split_path( filename );

	strstart(depname,maxlen-3); /* leave space for extension */
	stradd("dep/");
	stradd(ptr);
	strend();

	{
		char *ext = extension(depname);
		strncpy(ext,".d",3);
	}
}

void convert_to_uicname(char* uiname,int maxlen,const char* filename, const char* ext)
{
	const char* ptr = split_path( filename );
	
	strstart(uiname,maxlen-3); /* leave space for extension */
	stradd("uic/");
	stradd_range( ptr, extension((char*)ptr) );
	stradd( ext );
	strend();
}
