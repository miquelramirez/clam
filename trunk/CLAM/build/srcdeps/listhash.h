#ifndef __listhash__
#define __listhash__

#include "list.h"

typedef struct listkey
{
	char* str;
	struct listkey* next;
	list* l;
	int owns_list;
} listkey;

typedef struct listhash
{
	listkey* first;
	listkey* last;
} listhash;

listkey* listkey_new(const char* str);
void  listkey_free(listkey* i);

void  listkey_add_item(listkey* i,item* j);
void  listkey_add_item_str(listkey* i,const char* str);
void  listkey_add_item_str_once(listkey* i,const char* str);

listhash* listhash_new(void);
void  listhash_free(listhash* l);

int   listhash_size(listhash* l);

listkey* listhash_add(listhash* l,listkey* i);
listkey* listhash_add_key(listhash* l,const char* str);

listkey* listhash_add_in_front(listhash* l,listkey* i);
listkey* listhash_add_key_in_front(listhash* l,const char* str);

listkey* listhash_add_key_once(listhash* l,const char* str);

listkey* listhash_find(listhash* l,const char* str);

void  listhash_add_item(listhash* i,const char* keystr,item* j);
void  listhash_add_item_str(listhash* i,const char* keystr,const char* str);
void  listhash_add_item_str_once(listhash* i,const char* keystr,const char* str);

void  listhash_add_list(listhash* i,const char* keystr,list* l);

void  listhash_rmv_after(listhash* l,listkey* prev,listkey* i);
void  listhash_rmv_front(listhash* l);

#endif
