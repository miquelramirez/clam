#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack* stack_new(void)
{
	stack* ret = (stack*) malloc(sizeof(stack));
	ret->l = list_new();
	return ret;
}

void stack_free(stack* s)
{
	if (s)
	{
		list_free(s->l);
		free(s);
	}
}

void stack_push(stack* s,const char* val)
{
	list_add_str_in_front(s->l,val);
}

const char* stack_top(stack* s)
{
	return s->l->first ? s->l->first->str : 0;
}

void stack_pop(stack* s)
{
	list_rmv_front(s->l);
}
