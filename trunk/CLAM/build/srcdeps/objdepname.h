#ifndef __objdepname__
#define __objdepname__

char* extension(char* filename);

void extract_path( char* path, int maxLen, const char* filename );
void discard_path( char* newFilename, int maxLen, const char* filename );

const char* split_path( const char* path );

void convert_to_objname(char* objname,int maxlen,const char* filename);
void convert_to_depname(char* depname,int maxlen,const char* filename);
void convert_to_mocname(char* mocname,int maxlen,const char* filename);
void convert_to_uicname(char* uicname,int maxlen,const char* filename, const char* extension );


#endif
