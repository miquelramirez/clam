#ifndef __tree__
#define __tree__

typedef struct node
{
	char* str;
	struct node* next;
	struct tree* sub;
} node;

typedef struct tree
{
	node* first;
	node* last;
} tree;

node* node_new(const char* str);
void  node_free(node* i);

void  node_add_sub(node* i,node* j);
void  node_add_sub_str(node* i,const char* str);
void  node_add_sub_str_once(node* i,const char* str);

tree* tree_new(void);
void  tree_free(tree* l);

int   tree_size(tree* l);

node* tree_add(tree* l,node* i);
node* tree_add_str(tree* l,const char* str);

node* tree_add_in_front(tree* l,node* i);
node* tree_add_str_in_front(tree* l,const char* str);

node* tree_add_str_once(tree* l,const char* str);

node* tree_find(tree* l,const char* str);

void  tree_rmv_after(tree* l,node* prev,node* i);
void  tree_rmv_front(tree* l);

#endif
