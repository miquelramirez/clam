#ifndef __objdepname__
#define __objdepname__

char* extension(char* filename);

void convert_to_objname(char* objname,int maxlen,const char* filename);
void convert_to_depname(char* depname,int maxlen,const char* filename);

#endif
