#ifndef __hash__
#define __hash__

/* a poor man's hash. just a list of key=value pairs */

#include "list.h"

typedef struct hash
{
	list* l;
} hash;

/* create a new hash */
hash* hash_new(void);

/* free a hash */
void hash_free(hash* h);

/* add an entry to the hash, don't check for reoccurance */
void hash_add(hash* h,const char* key,const char* val);

/* remove an entry to the hash, don't check for reoccurance */
void hash_rmv(hash* h,const char* key);

/* add an entry to the hash, or change it's value when already existing */
void hash_set(hash* h,const char* key,const char* val);

/* get a value for a key from the hash */
const char* hash_value(hash* h,const char* key);

/* get a value for an item in the hash if the key matches */
const char* hash_item_value(hash* h,item* i,const char* key);

#endif
