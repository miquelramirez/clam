#ifndef __conditions__
#define __conditions__

void conditions_start(void);
void conditions_end(void);

int conditions_total(void);

void conditions_push(int v);

void conditions_pop(void);

void conditions_invert(void);

#endif
