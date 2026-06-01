#ifndef _TEST_FSM_H
#define _TEST_FSM_H

#include "fsm.h"

bool check_func(fsm_t* f);
bool check_func2(fsm_t* f);
void output_func(fsm_t* f);

void* fsm_malloc(size_t s);
void fsm_free(void*);

#endif
