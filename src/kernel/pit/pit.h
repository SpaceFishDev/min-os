#ifndef PIT_H
#define PIT_H
#include "../utils/util.h"
#include "../utils/debug/debug.h"

#define PIT_NATURAL_FREQ 1193180

#define PIT_DATA0 0x40
#define PIT_DATA1 0x41
#define PIT_DATA2 0x42
#define PIT_COMMAND 0x43

void init_timer(uint32 hz, void *callback);

#endif