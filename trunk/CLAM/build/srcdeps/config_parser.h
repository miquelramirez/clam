#ifndef __configparser__
#define __configparser__

#include "listhash.h"

extern listhash* config;
extern list* used_vars;

extern list* cxxflags_debug;
extern list* cxxflags_release;
extern list* link_flags_debug;
extern list* link_flags_release;
extern list* libraries_debug;
extern list* libraries_release;
extern list* vc7_libraries_debug;
extern list* vc7_libraries_release;

extern list* library_paths;
extern list* ui_files;

extern list* program;

int config_parse(const char* filename);
void config_apply(void);
void config_init(void);
void config_check(void);
void config_exit(void);

#endif
