#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listhash.h"
#include "strdup.h"

listkey* listkey_new(const char* str)
{
	listkey* ret = (listkey*) malloc(sizeof(listkey));
	ret->str = strdup(str);
	ret->next = 0;
	ret->l = 0;
	ret->owns_list = 1;
	return ret;
}

void listkey_free(listkey* i)
{
	if (i->str)
		free(i->str);
	if (i->owns_list)
		if (i->l)
			list_free(i->l);
	free(i);
}

void  listkey_add_item(listkey* i,item* j)
{
	if (!i->l)
		i->l = list_new();
	
	list_add(i->l,j);
}

void  listkey_add_item_str(listkey* i,const char* str)
{
	if (!i->l)
		i->l = list_new();
	
	list_add_str(i->l,str);
}

void  listkey_add_item_str_once(listkey* i,const char* str)
{
	if (!i->l)
		i->l = list_new();
	
	list_add_str_once(i->l,str);
}

listhash* listhash_new(void)
{
	listhash* ret = (listhash*) malloc(sizeof(listhash));
	ret->first = ret->last = 0;
	return ret;
}

void listhash_free(listhash* l)
{
	listkey *i = l->first;
	while (i)
	{
		listkey* n = i->next;
		listkey_free(i);
		i = n;
	}
	free(l);
}

listkey* listhash_add(listhash* l,listkey* i)
{
	if (l->last) l->last->next = i;
	else l->first = i;
	l->last = i;
	return i;
}

listkey* listhash_add_in_front(listhash* l,listkey* i)
{
	i->next = l->first;
	l->first = i;
	return i;
}

int listhash_size(listhash* l)
{
	int n = 0;
	listkey* i = l->first;
	while (i)
	{
		 i = i->next;
		 n++;
	}
	return n;
}

listkey* listhash_find(listhash* l,const char* str)
{
	listkey* i = l->first;
	
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

listkey* listhash_add_key(listhash* l,const char* str)
{
	listkey* i = listkey_new(str);
	listhash_add(l,i);
	return i;
}

listkey* listhash_add_key_in_front(listhash* l,const char* str)
{
	listkey* i = listkey_new(str);
	listhash_add_in_front(l,i);
	return i;
}

listkey* listhash_add_key_once(listhash* l,const char* str)
{
	listkey *i = listhash_find(l,str);
	if (i==0)
	{
		i = listkey_new(str);
		listhash_add(l,i);
	}
	return i;
}

void  listhash_add_item(listhash* i,const char* keystr,item* j)
{
	listkey* k = listhash_add_key_once(i,keystr);
	listkey_add_item(k,j);
}

void  listhash_add_item_str(listhash* i,const char* keystr,const char* str)
{
	listkey* k = listhash_add_key_once(i,keystr);
	listkey_add_item_str(k,str);
}

void  listhash_add_item_str_once(listhash* i,const char* keystr,const char* str)
{
	listkey* k = listhash_add_key_once(i,keystr);
	listkey_add_item_str_once(k,str);
}

void  listhash_add_list(listhash* i,const char* keystr,list* l)
{
	listkey* k = listhash_add_key_once(i,keystr);
	if (k->l)
	{
		fprintf(stderr,"ERROR: trying to create duplicate list for key %s\n",keystr);
	}
	k->l = l;
	k->owns_list = 0;
}

void listhash_rmv_after(listhash* l,listkey* prev,listkey* i)
{
	if (prev) prev->next = i->next;
	else l->first = i->next;
	if (i==l->last) l->last = prev;
	listkey_free(i);
}

void listhash_rmv_front(listhash* l)
{
	listkey* i = l->first;
	if (i)
	{
		if (i==l->last)
		{
			l->first = l->last = 0;	
		}else{
			l->first = i->next;
		}
		listkey_free(i);
	}
}
  
