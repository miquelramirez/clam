#ifndef __list__
#define __list__

typedef struct item
{
	char* str;
	struct item* next;
} item;

typedef struct list
{
	item* first;
	item* last;
	int lock;
} list;

item* item_new(const char* str);
void  item_free(item* i);

list* list_new(void);
void  list_free(list* l);
void  list_clear(list* l);

void  list_lock(list* l);
void  list_unlock(list* l);


int   list_size(list* l);

void  list_add(list* l,item* i);
void  list_add_str(list* l,const char* str);

void  list_add_in_front(list* l,item* i);
void  list_add_str_in_front(list* l,const char* str);

void  list_add_str_once(list* l,const char* str);

item* list_find(list* l,const char* str);

void  list_rmv_after(list* l,item* prev,item* i);
void  list_rmv_front(list* l);

#endif
