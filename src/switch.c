#include "switch.h"

bool is_debug_on(void) { return debug_on; }

void flip_debug_switch(void) { debug_on = !debug_on; }
