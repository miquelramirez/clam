#ifndef __parser__
#define __parser__

#include "list.h"
#include "listhash.h"
#include "tree.h"

extern list* pre_includes;
extern list* guessed_sources;
extern list* guessed_headers;
extern list* defines;
extern list *includes;
extern list *includepaths;
extern list *needed_includepaths;
extern listhash *ui_outputs;
extern list* mocable_headers;

extern int gendepend;
extern int recursesrcs;

void parser_init(void);
void parser_init_includes(void);
void parser_exit(void);

void parser_run(const char* filename);

// Function for building a tree following the paths in file lists
// This is used in dsp_parser and vcproj_parser
void generate_files_tree(list* filelist, tree* t);

typedef enum { source=0, header, qt } FileType;
const char* filetype_str(FileType type);

#endif

