#include "hash.h"
#include "strfuncs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hash* hash_new(void)
{
	hash* ret = (hash*) malloc(sizeof(hash));
	ret->l = list_new();
	ret->l->first = ret->l->last = 0;
	return ret;
}

void hash_free(hash* h)
{
	if (h)
	{
		list_free(h->l);
		free(h);
	}
}


void hash_add(hash* h,const char* key,const char* val)
{
	char tmp[2048];
	strstart(tmp,2048);
	stradd(key);
	stradd("=");
	stradd(val);
	strend();
	list_add_str(h->l,tmp);
}

const char* _chk_key(const char* a, const char* b)
{
	while (*a==*b) { a++; b++; }
	if (*a=='=' && *b=='\0')
	{
		return ++a;
	}
	return 0;
}

void hash_rmv(hash* h,const char* key)
{
	item* prev = 0;
	item* i = h->l->first;
	while (i)
	{
		if (_chk_key(i->str,key))
		{
			list_rmv_after(h->l, prev, i);
			return;
		}
		
		prev = i;
		i = i->next;
	}
}

void hash_set(hash* h,const char* key,const char* val)
{
	item* i = h->l->first;
	while (i)
	{
		if (_chk_key(i->str,key))
		{
			char tmp[2048];
			strstart(tmp,2048);
			stradd(key);
			stradd("=");
			stradd(val);
			strend();
			free(i->str);
			i->str = mystrdup(tmp);
			return;			
		}
		i = i->next;
	}
	hash_add(h,key,val);
}

const char* hash_item_value(hash* h,item* i,const char* key) // returns value or NULL
{
	return _chk_key(i->str,key);
}

const char* hash_value(hash* h,const char* key) // returns value or NULL
{
	item* i = h->l->first;
	while (i)
	{
		const char* ret = _chk_key(i->str,key);
		if (ret) return ret;
		i = i->next;
	}
	return 0;
}
