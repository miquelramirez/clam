#ifndef __strfuncs__
#define __strfuncs__

/* a more efficient way of catting strings than strcat */

/* set the ptr where we will be adding, and the max. length */
void strstart(char* tgt,int n);

char* stradd(const char* a);

char* stradd_range( const char* begin, const char* end );

void str_end(void);

/* Converts a given path from the UNIX way: a/b/c into 
 * Windows' way, namely a\b\c
 */
void winstyle(char* str);

#endif
