#ifndef FRAME_H
#define FRAME_H
#include "../utils/debug/debug.h"
#include "../utils/util.h"
#include "paging.h"
void init_frame_allocator(uint32 system_frames);
void alloc_frame(page *page, int is_kernel, int is_writeable);
void free_frame(page *page);

#endif