#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mystrdup.h"

node* node_new(const char* str)
{
	node* ret = (node*) malloc(sizeof(node));
	ret->str = mystrdup(str);
	ret->next = 0;
	ret->sub = 0;
	return ret;
}

void node_free(node* i)
{
	if (i->str)
		free(i->str);
	if (i->sub)
		tree_free(i->sub);
	free(i);
}

void  node_add_sub(node* i,node* j)
{
	if (!i->sub)
		i->sub = tree_new();
	
	tree_add(i->sub,j);
}

void  node_add_sub_str(node* i,const char* str)
{
	if (!i->sub)
		i->sub = tree_new();
	
	tree_add_str(i->sub,str);
}

void  node_add_sub_str_once(node* i,const char* str)
{
	if (!i->sub)
		i->sub = tree_new();
	
	tree_add_str_once(i->sub,str);
}

tree* tree_new(void)
{
	tree* ret = (tree*) malloc(sizeof(tree));
	ret->first = ret->last = 0;
	return ret;
}

void tree_free(tree* l)
{
	node *i = l->first;
	while (i)
	{
		node* n = i->next;
		node_free(i);
		i = n;
	}
	free(l);
}

node* tree_add(tree* l,node* i)
{
	if (l->last) l->last->next = i;
	else l->first = i;
	l->last = i;
	return i;
}

node* tree_add_in_front(tree* l,node* i)
{
	i->next = l->first;
	l->first = i;
	return i;
}

int tree_size(tree* l)
{
	int n = 0;
	node* i = l->first;
	while (i)
	{
		 i = i->next;
		 n++;
	}
	return n;
}

node* tree_find(tree* l,const char* str)
{
	node* i = l->first;
	
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

node* tree_add_str(tree* l,const char* str)
{
	node* i = node_new(str);
	tree_add(l,i);
	return i;
}

node* tree_add_str_in_front(tree* l,const char* str)
{
	node* i = node_new(str);
	tree_add_in_front(l,i);
	return i;
}

node* tree_add_str_once(tree* l,const char* str)
{
	node *i = tree_find(l,str);
	if (i==0)
	{
		i = node_new(str);
		tree_add(l,i);
	}
	return i;
}

void tree_rmv_after(tree* l,node* prev,node* i)
{
	if (prev) prev->next = i->next;
	else l->first = i->next;
	if (i==l->last) l->last = prev;
	node_free(i);
}

void tree_rmv_front(tree* l)
{
	node* i = l->first;
	if (i)
	{
		if (i==l->last)
		{
			l->first = l->last = 0;	
		}else{
			l->first = i->next;
		}
		node_free(i);
	}
}
  
