#ifndef __strfuncs__
#define __strfuncs__

/* a more efficient way of catting strings than strcat */

/* set the ptr where we will be adding, and the max. length */
void strstart(char* tgt,int n);

char* stradd(const char* a);

char* strend(void);

#endif
