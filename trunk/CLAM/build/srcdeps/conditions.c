#include <stdio.h>
#include <stdlib.h>


#include "verbose.h"

int condi = -1;
char cond[2048];

void conditions_start(void)
{
	condi = -1;
}

void conditions_end(void)
{
	if (condi!=-1)
	{
		fprintf(stderr,"Error: Unterminated preprocessor conditions\n");
	}
}

int conditions_total(void)
{
	int i;
	if (verbose) fprintf(stderr,"conditions_total: \n");

	for (i=condi;i>=0;i--) {
		if (verbose) fprintf(stderr,"[%d]%d",i,cond[i]);
		if (cond[i]==0) {
			if (verbose) fprintf(stderr,"=> 0\n");
			return 0;
		}
	}	
	if (verbose) fprintf(stderr,"=> 1\n");
	return 1;
}

void conditions_push(int v)
{
	condi++;
	if (verbose) fprintf(stderr,"conditions_push %d\n",condi);
	if (condi==2048) {
		fprintf(stderr,"Error: hard-coded limit of 2048 preprocessor conditions reached\n");
		exit(-1);
	}
	cond[condi] = v;
}

void conditions_pop(void)
{
	if (condi==-1) { 
		fprintf(stderr,"Error: Unmatched preprocessor directive\n");
		exit(-1);
	}
	condi--;
	if (verbose) fprintf(stderr,"conditions_pop %d\n",condi);
}

void conditions_invert(void)
{
	if (verbose) fprintf(stderr,"conditions_invert %d\n",condi);
	cond[condi] = (!cond[condi]);
}

