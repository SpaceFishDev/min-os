#ifndef HEAP_H

#define HEAP_H

#include "../utils/util.h"
#include "../paging/frame.h"

typedef struct heap_block
{
    uint32 size;
    bool free;
    char *ptr;
} heap_block;

#endif