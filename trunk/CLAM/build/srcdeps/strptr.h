#ifndef __strptr__
#define __strptr__

/* several string manipulation functions */

const char* strptr_skip_end(const char* ptr);
const char* strptr_skip_spaces(const char* ptr);
const char* strptr_copy_nonspaces(const char* ptr,char* tgt,int max);
const char* strptr_copy_until(const char* ptr,char term,char* tgt,int max);

#endif


