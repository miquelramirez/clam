#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "mystrdup.h"

item* item_new(const char* str)
{
	item* ret = (item*) malloc(sizeof(item));
	ret->str = mystrdup(str);
	ret->next = 0;
	return ret;
}

void item_free(item* i)
{
	if (i->str)
		free(i->str);
	free(i);
}

list* list_new(void)
{
	list* ret = (list*) malloc(sizeof(list));
	ret->first = ret->last = 0;
	ret->lock = 0;
	return ret;
}

void list_lock(list* l)
{
	l->lock++;
}

void list_unlock(list* l)
{
	l->lock--;
}

void list_free(list* l)
{
	if (l->lock)
	{
		fprintf(stderr,"Error: list lock %d != 0 when trying to free list\n",
			l->lock);
		fprintf(stderr,"  This suggests that the list is being multi-refered to\n");
		fprintf(stderr,"  Refusing to free list, clearing it only\n");
		list_clear(l);
	}else{
		list_clear(l);
		free(l);
	}
}

void list_clear(list* l)
{
	item *i = l->first;
	while (i)
	{
		item* n = i->next;
		item_free(i);
		i = n;
	}
	l->first = l->last = 0;
}

void list_add(list* l,item* i)
{
	if (l->last) l->last->next = i;
	else l->first = i;
	l->last = i;
}

void list_add_in_front(list* l,item* i)
{
	i->next = l->first;
	l->first = i;
	if (l->last==0) l->last = i;
}

int list_size(list* l)
{
	int n = 0;
	item* i = l->first;
	while (i)
	{
		 i = i->next;
		 n++;
	}
	return n;
}

item* list_find(list* l,const char* str)
{
	item* i = l->first;
	while (i)
	{
		const char* a = i->str;
		const char* b = str;
		while (*a && *b && *a==*b) { a++; b++; }
		if (*a=='\0' && *b=='\0')
		{
			return i;
		}
		i = i->next;
	}
	return 0;
}

void list_add_str(list* l,const char* str)
{
	item* i = item_new(str);
	list_add(l,i);
}

void list_add_str_in_front(list* l,const char* str)
{
	item* i = item_new(str);
	list_add_in_front(l,i);
}

void list_add_str_once(list* l,const char* str)
{
	if (!list_find(l,str))
	{
		item* i = item_new(str);
		list_add(l,i);
	}
}

void list_rmv_after(list* l,item* prev,item* i)
{
	if (prev) prev->next = i->next;
	else l->first = i->next;
	if (i==l->last) l->last = prev;
	item_free(i);
}

void list_rmv_front(list* l)
{
	item* i = l->first;
	if (i)
	{
		if (i==l->last)
		{
			l->first = l->last = 0;	
		}else{
			l->first = i->next;
		}
		item_free(i);
	}
}
