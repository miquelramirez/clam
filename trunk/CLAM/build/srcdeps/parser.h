#ifndef __parser__
#define __parser__

#include "list.h"

extern list* pre_includes;
extern list* guessed_sources;
extern list* guessed_headers;
extern list* defines;
extern list *includes;
extern list *includepaths;
extern list *needed_includepaths;

extern int gendepend;
extern int recursesrcs;

void parser_init(void);
void parser_init_includes(void);
void parser_exit(void);

void parser_run(const char* filename);



#endif
