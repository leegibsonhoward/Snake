#ifndef SWITCH_H
#define SWITCH_H

#include <stdio.h>
#include <stdbool.h>

bool is_debug_on(void);
void flip_debug_switch(void);
extern bool debug_on;

#endif
