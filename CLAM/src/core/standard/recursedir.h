#ifndef __recursedir__
#define __recursedir__

/* These are the type definitions of the function that are
** passed to recursedir to specify what to do on each directory
** and each file. Note that you can also pass NULL.
**
** The functions return a value to specify whether to stop (1)
** or to continue (0)
*/

typedef int (*on_file_func)(const char* filename,void* ptr);
typedef int (*on_dir_func)(const char* dir,void* ptr);


/* Recurse directory tree, starting at dir. max_recursion is the
** maximum level of recursion, where 0 means only visit dir itself,
** and -1 means any level of recursion. ptr is a void* that will be
** passed to the on_file_func and on_dir_func functions.
*/
int recursedir(
  const char* dir,int max_recursion,on_file_func f,on_dir_func d,void* ptr);

#endif
