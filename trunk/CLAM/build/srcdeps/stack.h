#ifndef __stack__
#define __stack__

#include "list.h"

typedef struct stack
{
	struct list* l;
} stack;

/* create a new stack */
stack* stack_new(void);

/* free a stack */
void stack_free(stack* s);

/* add an entry to the stack */
void stack_push(stack* s,const char* val);

/* access the last added entry on the stack */
const char* stack_top(stack* s);

/* remove last added entry from the stack*/
void stack_pop(stack* s);

#endif
