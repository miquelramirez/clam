#include "strptr.h"
#include <stdio.h>

const char* strptr_skip_end(const char* ptr)
{
	while (*ptr) ptr++;
	return ptr;
}

const char* strptr_skip_spaces(const char* ptr)
{
	while (*ptr && (*ptr==' ' || *ptr=='\t' || *ptr=='\n' || *ptr=='\r'))
		ptr++;
	return ptr;
}

const char* strptr_copy_nonspaces(const char* ptr,char* tgt,int max)
{
	int i = 0;
	char* start = tgt;
	while (*ptr && *ptr!=' ' && *ptr!='\t' && *ptr!='\n' && *ptr!='\r')
	{
		i++;
		if (i==max) {
			*tgt = 0;
			fprintf(stderr,"Error: not enough space in target string to copy %s...\n",start);
		}
		*tgt++ = *ptr++;
	}
	*tgt = 0;
	return ptr;
}

const char* strptr_copy_until(const char* ptr,char term,char* tgt,int max)
{
	int i = 0;
	char* start = tgt;
	while (*ptr && (*ptr!=term))
	{
		i++;
		if (i==max) {
			*tgt = 0;
			fprintf(stderr,"Error: not enough space in target string to copy %s...\n",start);
		}
		*tgt++ = *ptr++;
	}
	*tgt = 0;
	return ptr;
}

