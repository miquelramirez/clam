#include "strfuncs.h"
#include <stdio.h>
#include <stdlib.h>

char* cur;
int curn;

void strstart(char* tgt,int n)
{
	if (cur)
	{
		fprintf(stderr,"error: can only stradd one string at a time\n");
		exit(-1);
	}
	cur = tgt;
	curn = n;
	if (n)
		*cur = 0;
}

char* stradd(const char* a)
{
	if (a) while  (*a && curn--) { *cur++ = *a++; }
	*cur = 0;
	return cur;
}

char* strend(void)
{
	char* ret = cur;
	*cur = 0;
	cur = 0;
	return ret;
}
