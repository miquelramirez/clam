#include "mystrdup.h"
#include <stdlib.h>
#include <string.h>

char* mystrdup(const char* str)
{
	char* cpy = malloc(strlen(str)+1);
	if (str) 
	{
		strcpy(cpy,str);
	}
	return cpy;
}


